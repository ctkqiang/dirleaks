#include <stdio.h>
#include "../include/scanner.h"
#include "../include/http.h"

void scan_group(const char *group_name, const char **paths, int count, const char *url) {
    printf("\n--- 扫描 %s ---\n", group_name);

    for (int i = 0; i < count; i++) {
        int status = http_check(url, paths[i]);
    
        if (status == 200) {
            printf("[x] %-25s [找到] 状态: %d\n", paths[i], status);
        }
        
        if (status > 0) {
            printf("[ ] %-25s [未找到] 状态: %d\n", paths[i], status);
        }
        
        printf("[ ] %-25s [错误]\n", paths[i]);
    }
}
