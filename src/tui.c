#include <stdio.h>
#include <string.h>
#include "../include/tui.h"

int show_banner() {
    printf("╭═══════════════════════════════════════════════╮\n");
    printf("│               渗透测试工具 v1.0              │\n");
    printf("│                 by 钟智强 <ctkqiang> ♡                     │\n");
    printf("│  ⚖ 法律依据 & 合规提示                         │\n");
    printf("│  - 刑事诉讼法 第138条 (电子数据使用规定)       │\n");
    printf("│  - 网络安全法 第50条 (运营者安全义务)          │\n");
    printf("│  - 公安机关办理刑事案件程序规定 第234条        │\n");
    printf("│  - 电子签名法 第14条 (电子签名效力)           │\n");
    printf("│  - 《电子数据取证规范》GA/T 1070-2013        │\n");
    printf("│  - 移动互联网应用信息安全管理规定              │\n");
    printf("╰───────────────────────────────────────────────╯\n\n");
    return 0;
}

int show_menu() {
    int choice = -1;

    printf("=== 渗透工具向导菜单 ===\n");
    printf("0) 扫描所有路径\n");
    printf("1) C#\n");
    printf("2) Docker\n");
    printf("3) 通用路径\n");
    printf("4) Java\n");
    printf("5) Linux\n");
    printf("6) PHP\n");
    printf("7) Python\n");
    printf("8) Windows\n");
    printf("9) SQL\n");
    printf("10) C++\n");
    printf("11) Delphi\n");
    printf("12) Go\n");
    printf("13) Ruby\n");
    printf("14) Erlang & Elixir\n");
    printf("15) Wordpress\n");

    printf("请选择 [0-15]: ");
    
    scanf("%d", &choice);
    
    getchar();

    if (choice < 0 || choice > 8) choice = 0;
    return choice;
}

void ask_url(char *buffer, int size) {
    printf("请输入目标URL (例如 http://127.0.0.1:8080): ");

    fgets(buffer, size, stdin);
    
    buffer[strcspn(buffer, "\n")] = 0; 
}
