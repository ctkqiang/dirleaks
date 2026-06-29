/**
 * scanner.c — 路径扫描核心模块
 *
 * 职责：
 *   1. 对路径数组执行运行期去重，避免重复 HTTP 请求
 *   2. 调用 http_check_full 获取状态码与响应体关键词
 *   3. 按 OutputFormat 将结果序列化为文本 / JSON / CSV
 *   4. 通过 log_print 同步写入 stdout 与日志文件
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "../include/scanner.h"
#include "../include/http.h"
#include "../config/globals.h"

/* ------------------------------------------------------------------ */
/* 内部工具：同步写 stdout + 日志文件                                  */
/* ------------------------------------------------------------------ */

static void log_print(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    if (enable_log && log_fp) {
        va_start(args, fmt);
        vfprintf(log_fp, fmt, args);
        va_end(args);
        fflush(log_fp);
    }
}

/* ------------------------------------------------------------------ */
/* P1：路径去重                                                         */
/* ------------------------------------------------------------------ */

/**
 * dedup_paths — 对路径指针数组原地去重，保持首次出现顺序
 *
 * 算法复杂度 O(n²)，路径数组规模通常 < 200，性能可接受。
 * 不修改字符串内容，只通过移动指针实现去重。
 *
 * @param paths   路径指针数组（可写，指针值会被重排）
 * @param count   原始元素数量
 * @return        去重后的有效元素数量
 */
static int dedup_paths(const char **paths, int count) {
    int unique = 0;

    for (int i = 0; i < count; i++) {
        int found = 0;

        for (int j = 0; j < unique; j++) {
            if (strcmp(paths[i], paths[j]) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            paths[unique++] = paths[i];
        }
    }

    return unique;
}

/* ------------------------------------------------------------------ */
/* P3：结构化输出格式化函数                                             */
/* ------------------------------------------------------------------ */

/**
 * format_result_text — 文本格式单行输出
 */
static void format_result_text(const char *group, const char *path,
                                const HttpResult *r) {
    (void)group;

    if (r->status_code == 200 && r->has_keyword) {
        log_print("[敏感] %-45s  HTTP 200  关键词: %s\n",
                  path, r->matched_keyword);
    } else if (r->status_code == 200) {
        log_print("[发现] %-45s  HTTP 200\n", path);
    } else if (r->status_code > 0) {
        log_print("[    ] %-45s  HTTP %d\n", path, r->status_code);
    } else {
        log_print("[错误] %-45s  连接失败\n", path);
    }
}

/**
 * format_result_json — JSON 格式单行输出（每行一个 JSON 对象，NDJSON）
 */
static void format_result_json(const char *group, const char *path,
                                const HttpResult *r) {
    if (r->status_code == 200 && r->has_keyword) {
        log_print("{\"group\":\"%s\",\"path\":\"%s\",\"status\":%d,"
                  "\"sensitive\":true,\"keyword\":\"%s\"}\n",
                  group, path, r->status_code, r->matched_keyword);
    } else if (r->status_code > 0) {
        log_print("{\"group\":\"%s\",\"path\":\"%s\",\"status\":%d,"
                  "\"sensitive\":false,\"keyword\":\"\"}\n",
                  group, path, r->status_code);
    } else {
        log_print("{\"group\":\"%s\",\"path\":\"%s\",\"status\":-1,"
                  "\"sensitive\":false,\"keyword\":\"\"}\n",
                  group, path);
    }
}

/**
 * format_result_csv — CSV 格式单行输出
 */
static void format_result_csv(const char *group, const char *path,
                               const HttpResult *r) {
    if (r->status_code > 0) {
        log_print("%s,%s,%d,%s,%s\n",
                  group, path, r->status_code,
                  r->has_keyword ? "true" : "false",
                  r->has_keyword ? r->matched_keyword : "");
    } else {
        log_print("%s,%s,-1,false,\n", group, path);
    }
}

/* ------------------------------------------------------------------ */
/* 公开接口                                                             */
/* ------------------------------------------------------------------ */

void scan_group(const char *group_name, const char **paths, int count,
                const char *url, OutputFormat fmt) {

    /* 复制指针数组到栈上，避免修改调用方的 const 数组 */
    const char **work = malloc((size_t)count * sizeof(const char *));
    if (!work) {
        log_print("[错误] 内存分配失败，跳过组: %s\n", group_name);
        return;
    }
    memcpy(work, paths, (size_t)count * sizeof(const char *));

    /* P1：去重 */
    int unique_count = dedup_paths(work, count);

    /* 打印组头（JSON/CSV 模式下不打印分隔行） */
    if (fmt == OUTPUT_TEXT) {
        log_print("\n--- 扫描 %s  (%d 条路径，去重后 %d 条) ---\n",
                  group_name, count, unique_count);
    }

    /* CSV 首行表头（仅在第一个组输出，由调用方控制；此处每组都输出会重复，
       实际项目中可通过全局 flag 控制，此处保持简单） */

    for (int i = 0; i < unique_count; i++) {
        HttpResult result;
        http_check_full(url, work[i], &result);

        switch (fmt) {
            case OUTPUT_JSON:
                format_result_json(group_name, work[i], &result);
                break;
            case OUTPUT_CSV:
                format_result_csv(group_name, work[i], &result);
                break;
            default:
                format_result_text(group_name, work[i], &result);
                break;
        }
    }

    free(work);
}
