#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "config/globals.h"
#include "include/scanner.h"
#include "include/tui.h"

#include "include/cpp_path.h"
#include "include/csharp_path.h"
#include "include/delphi_path.h"
#include "include/docker_path.h"
#include "include/erlang_path.h"
#include "include/generic_path.h"
#include "include/go_path.h"
#include "include/java_path.h"
#include "include/linux_path.h"
#include "include/php_path.h"
#include "include/python_path.h"
#include "include/ruby_path.h"
#include "include/sql_path.h"
#include "include/windows_path.h"
#include "include/wordpress_path.h"

bool enable_log = true;
FILE *log_fp = NULL;

/* 扫描组描述符：将菜单编号、组名、路径数组、路径数量绑定在一起 */
typedef struct {
  int menu_id;
  const char *group_name;
  const char **paths;
  int count;
} ScanGroup;

/*
 * init_log — 在 out/ 目录下按目标主机名创建日志文件
 *
 * 若 out/ 目录不存在则自动创建。
 * 若文件创建失败，禁用日志功能并打印错误信息。
 */
static void init_log(const char *url) {
  if (!enable_log)
    return;

  struct stat st = {0};
  if (stat("out", &st) == -1) {
    mkdir("out", 0700);
  }

  /* 从 URL 中提取主机名（去掉协议头和路径） */
  const char *proto_end = strstr(url, "://");
  const char *host_start = (proto_end != NULL) ? proto_end + 3 : url;

  char host[256];
  strncpy(host, host_start, sizeof(host) - 1);
  host[sizeof(host) - 1] = '\0';

  char *path_sep = strchr(host, '/');
  if (path_sep != NULL) {
    *path_sep = '\0';
  }

  char log_path[512];
  snprintf(log_path, sizeof(log_path), "out/%s.log", host);

  log_fp = fopen(log_path, "w");
  if (log_fp == NULL) {
    perror("无法创建日志文件");
    enable_log = false;
  }
}

static void close_log(void) {
  if (log_fp != NULL) {
    fclose(log_fp);
    log_fp = NULL;
  }
}

int main(void) {
  show_banner();

  char url[256];
  int choice = show_menu();
  ask_url(url, sizeof(url));

  init_log(url);

  /*
   * 扫描组注册表
   * menu_id == 0 表示"扫描全部"，通过下方循环统一处理。
   * 每个条目的 menu_id 与 show_menu() 中的编号严格对应。
   */
  const ScanGroup groups[] = {
      {1, "CSharp", csharp_paths, csharp_count},
      {2, "Docker", docker_paths, docker_count},
      {3, "通用路径", generic_paths, generic_count},
      {4, "Java", java_paths, java_count},
      {5, "Linux", linux_paths, linux_count},
      {6, "PHP", php_paths, php_count},
      {7, "Python", python_paths, python_count},
      {8, "Windows", windows_paths, windows_count},
      {9, "SQL", sql_paths, sql_count},
      {10, "C/C++", cpp_paths, cpp_count},
      {11, "Delphi", delphi_paths, delphi_count},
      {12, "Go", go_paths, go_count},
      {13, "Ruby", ruby_paths, ruby_count},
      {14, "Erlang & Elixir", erlang_paths, erlang_count},
      {15, "WordPress", wordpress_paths, wordpress_count},
  };

  const int group_count = (int)(sizeof(groups) / sizeof(groups[0]));

  for (int i = 0; i < group_count; i++) {
    if (choice == 0 || choice == groups[i].menu_id) {
      scan_group(groups[i].group_name, groups[i].paths, groups[i].count, url);
    }
  }

  close_log();
  return 0;
}
