/**
 * windows_path.h — Windows 平台敏感路径声明
 *
 * 本模块收录 Windows / IIS 环境下常见的敏感文件与目录路径，
 * 用于渗透测试中的目录遍历与信息泄露探测。
 *
 * 路径分类说明：
 *   - Windows 系统文件  : hosts、win.ini、system.ini 等系统配置
 *   - IIS Web 根目录    : inetpub/wwwroot 及默认欢迎页
 *   - Web 配置文件      : web.config（ASP.NET 应用核心配置）
 *   - 桌面配置文件      : desktop.ini（文件夹显示配置，常被忽略）
 *
 * 使用方式：
 *   在扫描模块中通过 windows_paths 数组与 windows_count 计数
 *   调用 scan_group() 进行批量 HTTP 探测。
 *
 * 法律声明：
 *   本工具仅限授权渗透测试使用，未经授权扫描目标系统属违法行为。
 */

#ifndef WINDOWS_PATH_H
#define WINDOWS_PATH_H

/**
 * windows_paths — Windows 平台敏感路径列表
 *
 * 数组元素为以 '\0' 结尾的 C 字符串，定义于 src/windows_path.c。
 * 调用方不得修改数组内容（const 限定）。
 */
extern const char *windows_paths[];

/**
 * windows_count — windows_paths 数组的有效元素数量
 *
 * 由 src/windows_path.c 中的 sizeof 表达式在编译期计算，
 * 避免手动维护计数与数组长度不一致的问题。
 */
extern const int windows_count;

#endif /* WINDOWS_PATH_H */
