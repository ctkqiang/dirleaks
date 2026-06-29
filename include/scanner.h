/**
 * scanner.h — 路径扫描模块公开接口
 *
 * 依赖：
 *   - http.h        提供 HttpResult 结构体与 http_check_full()
 *   - globals.h     提供 enable_log / log_fp 全局状态
 */

#ifndef SCANNER_H
#define SCANNER_H

/**
 * OutputFormat — 扫描结果输出格式
 *
 *   OUTPUT_TEXT  纯文本，适合人工阅读（默认）
 *   OUTPUT_JSON  NDJSON，每行一个 JSON 对象，适合程序解析
 *   OUTPUT_CSV   CSV，适合导入 Excel / pandas
 */
typedef enum {
    OUTPUT_TEXT = 0,
    OUTPUT_JSON,
    OUTPUT_CSV
} OutputFormat;

/**
 * scan_group — 扫描单个路径组
 *
 * 内部执行去重后，对每条路径调用 http_check_full()，
 * 并按 fmt 指定的格式输出结果。
 *
 * @param group_name  组名称（用于日志标题和 JSON/CSV 字段）
 * @param paths       路径字符串指针数组
 * @param count       数组元素数量
 * @param url         目标基础 URL（如 http://target.com）
 * @param fmt         输出格式
 */
void scan_group(const char *group_name, const char **paths, int count,
                const char *url, OutputFormat fmt);

#endif /* SCANNER_H */
