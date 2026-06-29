/**
 * http.c — HTTP 探测模块实现
 *
 * 实现三个层次的探测能力：
 *   1. http_check_full  — GET 请求 + 响应体关键词匹配（核心实现）
 *   2. http_check       — 向后兼容的轻量包装
 *   3. http_check_batch — 基于 curl multi interface 的并发批量探测
 *
 * 敏感关键词列表定义在本文件内部，如需扩展可移至配置文件。
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "../include/http.h"

/* ------------------------------------------------------------------ */
/* 敏感关键词列表                                                       */
/* ------------------------------------------------------------------ */

static const char *SENSITIVE_KEYWORDS[] = {
    "password",
    "passwd",
    "secret",
    "token",
    "api_key",
    "apikey",
    "DB_PASSWORD",
    "DB_PASS",
    "AWS_SECRET",
    "private_key",
    "BEGIN RSA PRIVATE KEY",
    "BEGIN OPENSSH PRIVATE KEY",
    "Authorization",
    "credentials",
    NULL
};

/* ------------------------------------------------------------------ */
/* 响应体写入回调（供 curl 调用）                                       */
/* ------------------------------------------------------------------ */

/**
 * BodyBuffer — 用于接收 curl 响应体的动态缓冲区
 */
typedef struct {
    char  *data;
    size_t len;
    size_t cap;
} BodyBuffer;

/**
 * body_write_cb — curl CURLOPT_WRITEFUNCTION 回调
 *
 * 最多写入 HTTP_BODY_SNIPPET_LEN 字节，超出部分丢弃，
 * 避免大响应体消耗过多内存。
 */
static size_t body_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    BodyBuffer *buf = (BodyBuffer *)userdata;
    size_t incoming = size * nmemb;

    size_t remaining = buf->cap - buf->len - 1;
    size_t to_copy = (incoming < remaining) ? incoming : remaining;

    if (to_copy > 0) {
        memcpy(buf->data + buf->len, ptr, to_copy);
        buf->len += to_copy;
        buf->data[buf->len] = '\0';
    }

    return incoming;
}

/* ------------------------------------------------------------------ */
/* 关键词匹配                                                           */
/* ------------------------------------------------------------------ */

/**
 * match_keywords — 在 body 中搜索敏感关键词
 *
 * 找到第一个匹配时将关键词复制到 out->matched_keyword 并返回 1。
 * 未找到返回 0。
 */
static int match_keywords(const char *body, HttpResult *out) {
    for (int i = 0; SENSITIVE_KEYWORDS[i] != NULL; i++) {
        if (strstr(body, SENSITIVE_KEYWORDS[i]) != NULL) {
            strncpy(out->matched_keyword, SENSITIVE_KEYWORDS[i],
                    HTTP_KEYWORD_LEN - 1);
            out->matched_keyword[HTTP_KEYWORD_LEN - 1] = '\0';
            return 1;
        }
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/* P2：http_check_full — GET 请求 + 关键词匹配                         */
/* ------------------------------------------------------------------ */

void http_check_full(const char *base_url, const char *path, HttpResult *out) {
    out->status_code      = -1;
    out->has_keyword      = 0;
    out->matched_keyword[0] = '\0';

    char full_url[1024];
    snprintf(full_url, sizeof(full_url), "%s%s", base_url, path);

    char body_buf[HTTP_BODY_SNIPPET_LEN + 1];
    memset(body_buf, 0, sizeof(body_buf));

    BodyBuffer buf = { body_buf, 0, HTTP_BODY_SNIPPET_LEN };

    CURL *curl = curl_easy_init();
    if (!curl) return;

    long response_code = -1;

    curl_easy_setopt(curl, CURLOPT_URL,           full_url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,        10L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
    /* 不发送 NOBODY，需要响应体 */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  body_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,      &buf);
    /* 静默模式：不输出进度条 */
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS,     1L);

    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        out->status_code = (int)response_code;

        if (out->status_code == 200 && buf.len > 0) {
            out->has_keyword = match_keywords(body_buf, out);
        }
    }

    curl_easy_cleanup(curl);
}

/* ------------------------------------------------------------------ */
/* 向后兼容包装                                                         */
/* ------------------------------------------------------------------ */

int http_check(const char *base_url, const char *path) {
    HttpResult r;
    http_check_full(base_url, path, &r);
    return r.status_code;
}

/* ------------------------------------------------------------------ */
/* P2：http_check_batch — curl multi interface 并发批量探测             */
/* ------------------------------------------------------------------ */

/**
 * BatchSlot — 并发槽，每个槽持有一个 easy handle 及其关联资源
 */
typedef struct {
    CURL       *easy;
    HttpResult *result;
    BodyBuffer  buf;
    char        body_data[HTTP_BODY_SNIPPET_LEN + 1];
    char        full_url[1024];
    int         in_use;
} BatchSlot;

void http_check_batch(const char *base_url, const char **paths, int count,
                      HttpResult *results, int max_conn) {

    if (max_conn <= 0) max_conn = 10;

    /* 初始化所有结果为失败状态 */
    for (int i = 0; i < count; i++) {
        results[i].status_code      = -1;
        results[i].has_keyword      = 0;
        results[i].matched_keyword[0] = '\0';
    }

    CURLM *multi = curl_multi_init();
    if (!multi) return;

    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, (long)max_conn);

    BatchSlot *slots = calloc((size_t)max_conn, sizeof(BatchSlot));
    if (!slots) {
        curl_multi_cleanup(multi);
        return;
    }

    int next_path  = 0;
    int in_flight  = 0;

    /* 填充初始批次 */
    for (int s = 0; s < max_conn && next_path < count; s++, next_path++) {
        BatchSlot *slot = &slots[s];

        memset(slot->body_data, 0, sizeof(slot->body_data));
        slot->buf.data = slot->body_data;
        slot->buf.len  = 0;
        slot->buf.cap  = HTTP_BODY_SNIPPET_LEN;
        slot->result   = &results[next_path];
        slot->in_use   = 1;

        snprintf(slot->full_url, sizeof(slot->full_url),
                 "%s%s", base_url, paths[next_path]);

        slot->easy = curl_easy_init();
        if (!slot->easy) { slot->in_use = 0; continue; }

        curl_easy_setopt(slot->easy, CURLOPT_URL,            slot->full_url);
        curl_easy_setopt(slot->easy, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(slot->easy, CURLOPT_TIMEOUT,        10L);
        curl_easy_setopt(slot->easy, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(slot->easy, CURLOPT_NOPROGRESS,     1L);
        curl_easy_setopt(slot->easy, CURLOPT_WRITEFUNCTION,  body_write_cb);
        curl_easy_setopt(slot->easy, CURLOPT_WRITEDATA,      &slot->buf);
        /* 将槽指针附加到 easy handle，方便完成时回查 */
        curl_easy_setopt(slot->easy, CURLOPT_PRIVATE,        slot);

        curl_multi_add_handle(multi, slot->easy);
        in_flight++;
    }

    /* 事件循环：持续驱动直到所有请求完成 */
    while (in_flight > 0) {
        int running = 0;
        curl_multi_perform(multi, &running);

        /* 收割已完成的请求 */
        CURLMsg *msg;
        int msgs_left = 0;
        while ((msg = curl_multi_info_read(multi, &msgs_left)) != NULL) {
            if (msg->msg != CURLMSG_DONE) continue;

            CURL *done_easy = msg->easy_handle;
            BatchSlot *slot = NULL;
            curl_easy_getinfo(done_easy, CURLINFO_PRIVATE, &slot);

            if (slot && msg->data.result == CURLE_OK) {
                long code = -1;
                curl_easy_getinfo(done_easy, CURLINFO_RESPONSE_CODE, &code);
                slot->result->status_code = (int)code;

                if (slot->result->status_code == 200 && slot->buf.len > 0) {
                    slot->result->has_keyword =
                        match_keywords(slot->body_data, slot->result);
                }
            }

            curl_multi_remove_handle(multi, done_easy);
            curl_easy_cleanup(done_easy);
            slot->in_use = 0;
            in_flight--;

            /* 补充下一条路径 */
            if (next_path < count) {
                memset(slot->body_data, 0, sizeof(slot->body_data));
                slot->buf.data = slot->body_data;
                slot->buf.len  = 0;
                slot->buf.cap  = HTTP_BODY_SNIPPET_LEN;
                slot->result   = &results[next_path];
                slot->in_use   = 1;

                snprintf(slot->full_url, sizeof(slot->full_url),
                         "%s%s", base_url, paths[next_path]);
                next_path++;

                slot->easy = curl_easy_init();
                if (slot->easy) {
                    curl_easy_setopt(slot->easy, CURLOPT_URL,            slot->full_url);
                    curl_easy_setopt(slot->easy, CURLOPT_FOLLOWLOCATION, 1L);
                    curl_easy_setopt(slot->easy, CURLOPT_TIMEOUT,        10L);
                    curl_easy_setopt(slot->easy, CURLOPT_CONNECTTIMEOUT, 5L);
                    curl_easy_setopt(slot->easy, CURLOPT_NOPROGRESS,     1L);
                    curl_easy_setopt(slot->easy, CURLOPT_WRITEFUNCTION,  body_write_cb);
                    curl_easy_setopt(slot->easy, CURLOPT_WRITEDATA,      &slot->buf);
                    curl_easy_setopt(slot->easy, CURLOPT_PRIVATE,        slot);
                    curl_multi_add_handle(multi, slot->easy);
                    in_flight++;
                }
            }
        }

        if (running > 0) {
            curl_multi_wait(multi, NULL, 0, 100, NULL);
        }
    }

    free(slots);
    curl_multi_cleanup(multi);
}
