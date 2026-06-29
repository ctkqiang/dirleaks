/**
 * dirleaks.c — 主入口
 *
 * 支持两种运行模式：
 *
 *   交互模式（无参数）：
 *     ./dirleaks
 *     依次显示横幅、菜单、URL 输入提示。
 *
 *   命令行模式（适合脚本/CI）：
 *     ./dirleaks -u <url> [-g <0-15>] [-f text|json|csv] [-c <并发数>]
 * [--no-log]
 *
 *     -u <url>      目标基础 URL（必填）
 *     -g <id>       扫描组编号 0-15，0 表示全部（默认 0）
 *     -f <format>   输出格式：text / json / csv（默认 text）
 *     -c <n>        并发连接数（默认 10）
 *     --no-log      禁用文件日志
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "config/globals.h"
#include "include/http.h"
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

/* ------------------------------------------------------------------ */
/* 扫描组注册表                                                         */
/* ------------------------------------------------------------------ */

/**
 * ScanGroup — 将菜单编号、组名、路径数组、路径数量绑定在一起
 */
typedef struct {
  int menu_id;
  const char *group_name;
  const char **paths;
  int count;
} ScanGroup;

/* GROUPS 在 build_groups() 中运行期填充，因为 extern const int 计数值
   不是 C 编译期常量，无法用于静态初始化器。 */
static ScanGroup GROUPS[15];
static int GROUP_COUNT = 0;

static void build_groups(void) {
  int i = 0;
  GROUPS[i++] = (ScanGroup){1, "CSharp", csharp_paths, csharp_count};
  GROUPS[i++] = (ScanGroup){2, "Docker", docker_paths, docker_count};
  GROUPS[i++] = (ScanGroup){3, "通用路径", generic_paths, generic_count};
  GROUPS[i++] = (ScanGroup){4, "Java", java_paths, java_count};
  GROUPS[i++] = (ScanGroup){5, "Linux", linux_paths, linux_count};
  GROUPS[i++] = (ScanGroup){6, "PHP", php_paths, php_count};
  GROUPS[i++] = (ScanGroup){7, "Python", python_paths, python_count};
  GROUPS[i++] = (ScanGroup){8, "Windows", windows_paths, windows_count};
  GROUPS[i++] = (ScanGroup){9, "SQL", sql_paths, sql_count};
  GROUPS[i++] = (ScanGroup){10, "C/C++", cpp_paths, cpp_count};
  GROUPS[i++] = (ScanGroup){11, "Delphi", delphi_paths, delphi_count};
  GROUPS[i++] = (ScanGroup){12, "Go", go_paths, go_count};
  GROUPS[i++] = (ScanGroup){13, "Ruby", ruby_paths, ruby_count};
  GROUPS[i++] = (ScanGroup){14, "Erlang & Elixir", erlang_paths, erlang_count};
  GROUPS[i++] = (ScanGroup){15, "WordPress", wordpress_paths, wordpress_count};
  GROUP_COUNT = i;
}

/* ------------------------------------------------------------------ */
/* 命令行参数                                                           */
/* ------------------------------------------------------------------ */

/**
 * CliArgs — 解析后的命令行参数
 *
 * interactive == 1 时忽略其他字段，进入交互模式。
 */
typedef struct {
  int interactive;
  char url[256];
  int group_id;
  OutputFormat format;
  int max_conn;
  int no_log;
} CliArgs;

static void print_usage(const char *prog) {
  printf("用法: %s [选项]\n\n", prog);
  printf("  无参数              进入交互模式\n\n");
  printf("  -u <url>            目标基础 URL（必填）\n");
  printf("  -g <0-15>           扫描组编号，0 表示全部（默认 0）\n");
  printf("  -f text|json|csv    输出格式（默认 text）\n");
  printf("  -c <n>              并发连接数（默认 10）\n");
  printf("  --no-log            禁用文件日志\n");
  printf("  -h                  显示此帮助\n\n");
  printf("示例:\n");
  printf("  %s -u http://target.com -g 4 -f json -c 20\n", prog);
  printf("  %s -u http://target.com -g 0 --no-log\n\n", prog);
}

/**
 * parse_args — 解析 argv，填充 CliArgs
 *
 * @return  0 成功，-1 参数错误（调用方应退出）
 */
static int parse_args(int argc, char *argv[], CliArgs *out) {
  out->interactive = 0;
  out->url[0] = '\0';
  out->group_id = 0;
  out->format = OUTPUT_TEXT;
  out->max_conn = 10;
  out->no_log = 0;

  if (argc == 1) {
    out->interactive = 1;
    return 0;
  }

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_usage(argv[0]);
      return -1;
    }

    if (strcmp(argv[i], "--no-log") == 0) {
      out->no_log = 1;
      continue;
    }

    if (strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
      strncpy(out->url, argv[++i], sizeof(out->url) - 1);
      out->url[sizeof(out->url) - 1] = '\0';
      continue;
    }

    if (strcmp(argv[i], "-g") == 0 && i + 1 < argc) {
      out->group_id = atoi(argv[++i]);
      if (out->group_id < 0 || out->group_id > GROUP_COUNT) {
        fprintf(stderr, "错误: -g 参数超出范围 [0-%d]\n", GROUP_COUNT);
        return -1;
      }
      continue;
    }

    if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
      const char *fmt = argv[++i];
      if (strcmp(fmt, "json") == 0) {
        out->format = OUTPUT_JSON;
      } else if (strcmp(fmt, "csv") == 0) {
        out->format = OUTPUT_CSV;
      } else if (strcmp(fmt, "text") == 0) {
        out->format = OUTPUT_TEXT;
      } else {
        fprintf(stderr, "错误: -f 参数无效，可选值: text / json / csv\n");
        return -1;
      }
      continue;
    }

    if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
      out->max_conn = atoi(argv[++i]);
      if (out->max_conn <= 0)
        out->max_conn = 10;
      continue;
    }

    fprintf(stderr, "错误: 未知参数 '%s'，使用 -h 查看帮助\n", argv[i]);
    return -1;
  }

  if (out->url[0] == '\0') {
    fprintf(stderr, "错误: 必须通过 -u 指定目标 URL\n");
    return -1;
  }

  return 0;
}

/* ------------------------------------------------------------------ */
/* 日志初始化与关闭                                                     */
/* ------------------------------------------------------------------ */

/**
 * init_log — 在 out/ 目录下按目标主机名创建日志文件
 */
static void init_log(const char *url) {
  if (!enable_log)
    return;

  struct stat st = {0};
  if (stat("out", &st) == -1) {
    mkdir("out", 0700);
  }

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

/* ------------------------------------------------------------------ */
/* 扫描分发                                                             */
/* ------------------------------------------------------------------ */

static void run_scan(const char *url, int group_id, OutputFormat fmt) {
  /* CSV 模式输出表头 */
  if (fmt == OUTPUT_CSV) {
    printf("group,path,status,sensitive,keyword\n");
    if (enable_log && log_fp) {
      fprintf(log_fp, "group,path,status,sensitive,keyword\n");
      fflush(log_fp);
    }
  }

  for (int i = 0; i < GROUP_COUNT; i++) {
    if (group_id == 0 || group_id == GROUPS[i].menu_id) {
      scan_group(GROUPS[i].group_name, GROUPS[i].paths, GROUPS[i].count, url,
                 fmt);
    }
  }
}

/* ------------------------------------------------------------------ */
/* 主入口                                                               */
/* ------------------------------------------------------------------ */

int main(int argc, char *argv[]) {
  CliArgs args;

  build_groups();

  if (parse_args(argc, argv, &args) != 0) {
    return 1;
  }

  if (args.interactive) {
    /* 交互模式 */
    show_banner();

    char url[256];
    int choice = show_menu();
    ask_url(url, sizeof(url));

    init_log(url);
    run_scan(url, choice, OUTPUT_TEXT);
  } else {
    /* 命令行模式 */
    if (args.no_log) {
      enable_log = false;
    }

    init_log(args.url);
    run_scan(args.url, args.group_id, args.format);
  }

  close_log();
  return 0;
}
