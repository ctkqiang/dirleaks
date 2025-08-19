#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "include/tui.h"
#include "include/scanner.h"
#include "config/globals.h"

#include "include/csharp_path.h"
#include "include/docker_path.h"
#include "include/generic_path.h"
#include "include/java_path.h"
#include "include/linux_path.h"
#include "include/php_path.h"
#include "include/python_path.h"
#include "include/windows_path.h"
#include "include/sql_path.h"
#include "include/delphi_path.h"
#include "include/cpp_path.h"
#include "include/go_path.h"
#include "include/ruby_path.h"
#include "include/erlang_path.h"
#include "include/wordpress_path.h"

bool enable_log = true;
FILE *log_fp = NULL;

static void init_log(const char *url) {
    if (!enable_log) return;

    struct stat st = {0};
    if (stat("out", &st) == -1) {
        mkdir("out", 0700);
    }

    char filename[512];
    const char *p = strstr(url, "://");
    const char *host = (p ? p + 3 : url);
    char host_copy[256];
    strncpy(host_copy, host, sizeof(host_copy) - 1);
    host_copy[sizeof(host_copy) - 1] = 0;

    char *slash = strchr(host_copy, '/');
    if (slash) *slash = 0;

    snprintf(filename, sizeof(filename), "out/%s.log", host_copy);

    log_fp = fopen(filename, "w");
    if (!log_fp) {
        perror("无法创建日志文件");
        enable_log = false;
    }
}

static void close_log() {
    if (log_fp) fclose(log_fp);
}

int main() {
    show_banner();

    char url[256];
    int choice = show_menu();
    ask_url(url, sizeof(url));

    init_log(url);

    if (choice == 0 || choice == 1) scan_group("CSharp", csharp_paths, csharp_count, url);
    if (choice == 0 || choice == 2) scan_group("Docker", docker_paths, docker_count, url);
    if (choice == 0 || choice == 3) scan_group("Generic", generic_paths, generic_count, url);
    if (choice == 0 || choice == 4) scan_group("Java", java_paths, java_count, url);
    if (choice == 0 || choice == 5) scan_group("Linux", linux_paths, linux_count, url);
    if (choice == 0 || choice == 6) scan_group("PHP", php_paths, php_count, url);
    if (choice == 0 || choice == 7) scan_group("Python", python_paths, python_count, url);
    if (choice == 0 || choice == 8) scan_group("Windows", windows_paths, windows_count, url);
    if (choice == 0 || choice == 9) scan_group("SQL", sql_paths, sql_count, url);
    if (choice == 0 || choice == 10) scan_group("C/C++", cpp_paths, cpp_count, url);
    if (choice == 0 || choice == 11) scan_group("Delphi", delphi_paths, delphi_count, url);
    if (choice == 0 || choice == 12) scan_group("Go", go_paths, go_count, url);
    if (choice == 0 || choice == 13) scan_group("Ruby", ruby_paths, ruby_count, url);
    if (choice == 0 || choice == 14) scan_group("Erlang & Elixir", erlang_paths, erlang_count, url);
    if (choice == 0 || choice == 14) scan_group("Wordpress", wordpress_paths,wordpress_count, url);

    close_log();
    return 0;
}
