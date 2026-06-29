/**
 * tui.c — 命令行交互界面模块
 *
 * 职责：
 *   1. show_banner()  — 显示工具横幅与法律合规提示
 *   2. show_menu()    — 显示扫描组菜单，读取并验证用户选择
 *   3. ask_url()      — 读取目标 URL，去除末尾换行符
 */

#include <stdio.h>
#include <string.h>
#include "../include/tui.h"

/** 菜单有效选项上限，与 dirleaks.c 中 group_count 保持同步 */
#define MENU_MAX 15

int show_banner(void) {
    printf("============================================================\n");
    printf("  渗透测试工具 dirleaks  v1.0\n");
    printf("  作者: 钟智强 <ctkqiang>\n");
    printf("------------------------------------------------------------\n");
    printf("  法律依据与合规提示\n");
    printf("  - 刑事诉讼法 第138条    (电子数据使用规定)\n");
    printf("  - 网络安全法 第50条     (运营者安全义务)\n");
    printf("  - 公安机关办理刑事案件程序规定 第234条\n");
    printf("  - 电子签名法 第14条     (电子签名效力)\n");
    printf("  - 电子数据取证规范 GA/T 1070-2013\n");
    printf("  - 移动互联网应用信息安全管理规定\n");
    printf("============================================================\n\n");
    printf("  警告: 本工具仅限授权渗透测试使用。\n");
    printf("        未经授权扫描目标系统属违法行为。\n\n");
    return 0;
}

int show_menu(void) {
    int choice = -1;

    printf("------------------------------------------------------------\n");
    printf("  扫描组选择\n");
    printf("------------------------------------------------------------\n");
    printf("   0) 扫描全部路径\n");
    printf("   1) C#\n");
    printf("   2) Docker\n");
    printf("   3) 通用路径\n");
    printf("   4) Java\n");
    printf("   5) Linux\n");
    printf("   6) PHP\n");
    printf("   7) Python\n");
    printf("   8) Windows\n");
    printf("   9) SQL\n");
    printf("  10) C/C++\n");
    printf("  11) Delphi\n");
    printf("  12) Go\n");
    printf("  13) Ruby\n");
    printf("  14) Erlang & Elixir\n");
    printf("  15) WordPress\n");
    printf("------------------------------------------------------------\n");
    printf("  请选择 [0-%d]: ", MENU_MAX);

    if (scanf("%d", &choice) != 1) {
        choice = 0;
    }
    getchar();

    if (choice < 0 || choice > MENU_MAX) {
        printf("  输入超出范围，已默认选择 0（扫描全部）。\n");
        choice = 0;
    }

    return choice;
}

void ask_url(char *buffer, int size) {
    printf("  请输入目标 URL (例如 http://127.0.0.1:8080): ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}
