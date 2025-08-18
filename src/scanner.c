#include <stdio.h>
#include <stdarg.h>

#include "../include/scanner.h"
#include "../include/http.h"

#include "../config/globals.h"


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

void scan_group(const char *group_name, const char **paths, int count, const char *url) {
    log_print("\n--- 扫描 %s ---\n", group_name);

    for (int i = 0; i < count; i++) {
        int status = http_check(url, paths[i]);

        if (status == 200) {
            log_print("[x] %-25s [找到] 状态: %d\n", paths[i], status);
        } 
        
        if (status > 0) {
            log_print("[ ] %-25s [未找到] 状态: %d\n", paths[i], status);
        } 
     
        log_print("[ ] %-25s [错误]\n", paths[i]);
        
    }
}
