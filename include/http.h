/**
 * http.h — HTTP 探测模块公开接口
 *
 * 提供三个层次的 HTTP 检测能力：
 *   1. http_check()       — 轻量 HEAD 请求，仅返回状态码（向后兼容）
 *   2. http_check_full()  — GET 请求，返回状态码 + 响应体关键词匹配
 *   3. http_check_batch() — 并发批量 GET，基于 curl multi interface
 */

#ifndef HTTP_H
#define HTTP_H

/** 响应体片段最大长度（字节） */
#define HTTP_BODY_SNIPPET_LEN 256

/** 匹配到的关键词最大长度（字节） */
#define HTTP_KEYWORD_LEN 64

/**
 * HttpResult — 单次 HTTP 探测的完整结果
 *
 * status_code    HTTP 状态码；-1 表示连接失败或 curl 初始化失败
 * has_keyword    响应体中是否命中敏感关键词（仅 status_code == 200 时有效）
 * matched_keyword 命中的第一个关键词；未命中时为空字符串
 */
typedef struct {
    int  status_code;
    int  has_keyword;
    char matched_keyword[HTTP_KEYWORD_LEN];
} HttpResult;

/**
 * http_check — 轻量 HEAD 请求
 *
 * 向后兼容接口，内部调用 http_check_full 并只返回状态码。
 *
 * @param base_url  目标基础 URL
 * @param path      探测路径
 * @return          HTTP 状态码，-1 表示失败
 */
int http_check(const char *base_url, const char *path);

/**
 * http_check_full — GET 请求 + 响应体关键词匹配
 *
 * 仅在 status_code == 200 时读取响应体前 HTTP_BODY_SNIPPET_LEN 字节，
 * 并与内置敏感关键词列表比对。
 *
 * @param base_url  目标基础 URL
 * @param path      探测路径
 * @param out       输出结果，调用方负责提供有效指针
 */
void http_check_full(const char *base_url, const char *path, HttpResult *out);

/**
 * http_check_batch — 并发批量探测
 *
 * 使用 curl multi interface 并发发送请求，结果写入 results 数组。
 * results 数组长度须 >= count，由调用方分配。
 *
 * @param base_url  目标基础 URL
 * @param paths     路径指针数组
 * @param count     路径数量
 * @param results   输出结果数组
 * @param max_conn  最大并发连接数（建议 10–20，过大可能触发目标限速）
 */
void http_check_batch(const char *base_url, const char **paths, int count,
                      HttpResult *results, int max_conn);

#endif /* HTTP_H */
