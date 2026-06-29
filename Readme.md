# dirleaks — 敏感路径扫描工具

**dirleaks** 是一款轻量级、高效的敏感路径扫描工具，专为渗透测试人员、红队成员与安全研究员设计。  
基于 **C 语言 + libcurl** 实现，在信息收集阶段快速识别目标站点中暴露的敏感文件、配置文件、备份文件与目录。

> **法律声明：本工具仅限授权渗透测试、安全研究与教育学习使用。未经授权扫描目标系统属违法行为，使用者须自行承担全部法律责任。**

---

## 功能特性

- **15 个扫描组**：覆盖 CSharp、Docker、通用路径、Java、Linux、PHP、Python、Windows、SQL、C/C++、Delphi、Go、Ruby、Erlang & Elixir、WordPress
- **响应体关键词匹配**：对 HTTP 200 响应自动检测敏感关键词（`password`、`secret`、`token`、`api_key`、`AWS_SECRET`、`BEGIN RSA PRIVATE KEY` 等 14 个）
- **并发扫描**：基于 `curl multi interface`，默认 10 路并发，可通过 `-c` 参数调整
- **路径去重**：扫描前自动对路径数组去重，避免重复请求
- **三种输出格式**：`text`（人工阅读）、`json`（NDJSON，适合程序解析）、`csv`（适合导入 Excel / pandas）
- **两种运行模式**：交互式菜单模式 与 命令行参数模式（适合脚本与 CI/CD）
- **文件日志**：扫描结果自动写入 `out/<目标主机名>.log`，支持 `--no-log` 禁用

---

## 依赖环境

| 依赖 | 版本要求 | 安装方式 |
|------|----------|----------|
| GCC  | >= 7     | 系统自带或包管理器安装 |
| libcurl | >= 7.50 | 见下方说明 |

```bash
# macOS
brew install curl

# Debian / Ubuntu
sudo apt-get install libcurl4-openssl-dev

# CentOS / RHEL
sudo yum install libcurl-devel
```

---

## 编译

```bash
make
```

清理构建产物：

```bash
make clean
```

---

## 使用方法

### 交互模式

无参数运行，进入交互式菜单：

```bash
./dirleaks
```

```
============================================================
  渗透测试工具 dirleaks  v1.0
  作者: 钟智强 <ctkqiang>
------------------------------------------------------------
  法律依据与合规提示
  ...
============================================================

------------------------------------------------------------
  扫描组选择
------------------------------------------------------------
   0) 扫描全部路径
   1) C#
   2) Docker
   3) 通用路径
   4) Java
   5) Linux
   6) PHP
   7) Python
   8) Windows
   9) SQL
  10) C/C++
  11) Delphi
  12) Go
  13) Ruby
  14) Erlang & Elixir
  15) WordPress
------------------------------------------------------------
  请选择 [0-15]:
  请输入目标 URL (例如 http://127.0.0.1:8080):
```

### 命令行模式

适合脚本、自动化流水线与 CI/CD 环境，无需交互输入：

```bash
./dirleaks -u <url> [-g <0-15>] [-f text|json|csv] [-c <并发数>] [--no-log]
```

| 参数 | 说明 | 默认值 |
|------|------|--------|
| `-u <url>` | 目标基础 URL（必填） | — |
| `-g <0-15>` | 扫描组编号，0 表示全部 | `0` |
| `-f text\|json\|csv` | 输出格式 | `text` |
| `-c <n>` | 并发连接数 | `10` |
| `--no-log` | 禁用文件日志 | 默认启用 |
| `-h` | 显示帮助 | — |

**示例：**

```bash
# 扫描 Java 组，输出 JSON，20 路并发
./dirleaks -u http://target.com -g 4 -f json -c 20

# 扫描全部路径，输出 CSV，禁用日志
./dirleaks -u http://target.com -g 0 -f csv --no-log > result.csv

# 在 CI 中使用 jq 过滤出所有 HTTP 200 的路径
./dirleaks -u http://target.com -f json --no-log | jq 'select(.status == 200)'
```

---

## 输出格式说明

### text（默认）

```
--- 扫描 Java  (22 条路径，去重后 22 条) ---
[发现] /actuator/env                           HTTP 200
[敏感] /application.properties                HTTP 200  关键词: password
[    ] /WEB-INF/web.xml                        HTTP 403
[错误] /target/                                连接失败
```

| 标记 | 含义 |
|------|------|
| `[发现]` | HTTP 200，路径存在 |
| `[敏感]` | HTTP 200 且响应体命中敏感关键词 |
| `[    ]` | 其他 HTTP 状态码（403、404 等） |
| `[错误]` | 连接失败或超时 |

### json（NDJSON）

每行一个 JSON 对象，可直接用 `jq` 处理：

```json
{"group":"Java","path":"/actuator/env","status":200,"sensitive":false,"keyword":""}
{"group":"Java","path":"/application.properties","status":200,"sensitive":true,"keyword":"password"}
{"group":"Java","path":"/WEB-INF/web.xml","status":403,"sensitive":false,"keyword":""}
```

### csv

首行为表头，可直接导入 Excel 或 pandas：

```
group,path,status,sensitive,keyword
Java,/actuator/env,200,false,
Java,/application.properties,200,true,password
Java,/WEB-INF/web.xml,403,false,
```

---

## 日志文件

扫描结果默认写入：

```
out/<目标主机名>.log
```

例如扫描 `http://target.com` 时，日志路径为 `out/target.com.log`。  
使用 `--no-log` 参数可禁用此功能。

---

## 目录结构

```
dirleaks/
├── dirleaks.c              主入口：参数解析、日志初始化、扫描分发
├── config/
│   └── globals.h           全局状态：enable_log、log_fp
├── include/                头文件
│   ├── http.h              HTTP 探测接口（HttpResult、三层探测函数）
│   ├── scanner.h           扫描接口（scan_group、OutputFormat）
│   ├── tui.h               交互界面接口
│   ├── cpp_path.h
│   ├── csharp_path.h
│   ├── delphi_path.h
│   ├── docker_path.h
│   ├── erlang_path.h
│   ├── generic_path.h
│   ├── go_path.h
│   ├── java_path.h
│   ├── javascript_path.h
│   ├── linux_path.h
│   ├── php_path.h
│   ├── python_path.h
│   ├── ruby_path.h
│   ├── sql_path.h
│   ├── windows_path.h
│   └── wordpress_path.h
├── src/                    源码实现
│   ├── http.c              http_check / http_check_full / http_check_batch
│   ├── scanner.c           scan_group（去重 + 格式化输出）
│   ├── tui.c               show_banner / show_menu / ask_url
│   ├── cpp_path.c
│   ├── csharp_path.c
│   ├── delphi_path.c
│   ├── docker_path.c
│   ├── erlang_path.c
│   ├── generic_path.c
│   ├── go_path.c
│   ├── java_path.c
│   ├── javascript_path.c
│   ├── linux_path.c
│   ├── php_path.c
│   ├── python_path.c
│   ├── ruby_path.c
│   ├── sql_path.c
│   ├── windows_path.c
│   └── wordpress_path.c
├── test/
│   └── test_http.c         单元测试（cmocka mock libcurl）
├── .github/
│   └── workflows/
│       └── release.yml     CI：push tag 触发构建与 Release 发布
├── makefile
└── Readme.md
```

---

## 架构说明

```
用户输入（交互 / 命令行参数）
    └── main()
         ├── parse_args()       解析命令行参数
         ├── init_log()         创建日志文件
         ├── run_scan()         遍历扫描组注册表
         │    └── scan_group()  去重 → 逐路径探测 → 格式化输出
         │         └── http_check_full()
         │              ├── curl GET 请求（含超时控制）
         │              ├── 响应体写入 BodyBuffer（最多 256 字节）
         │              └── match_keywords()  敏感关键词匹配
         └── close_log()
```

---

## 单元测试

测试依赖 [cmocka](https://cmocka.org/) 框架，通过 mock libcurl 接口实现隔离测试：

```bash
# macOS
brew install cmocka

# Debian / Ubuntu
sudo apt-get install libcmocka-dev

# 编译并运行测试
gcc -Wall test/test_http.c -o test_http -lcmocka -Wl,--wrap=curl_easy_init \
    -Wl,--wrap=curl_easy_setopt -Wl,--wrap=curl_easy_perform \
    -Wl,--wrap=curl_easy_getinfo -Wl,--wrap=curl_easy_cleanup
./test_http
```

覆盖场景：HTTP 200、HTTP 404、`curl_easy_perform` 失败、`curl_easy_init` 失败。

---

## CI / CD

项目通过 GitHub Actions 自动构建与发布。  
推送 `v*` 格式的 tag 时触发：

1. 在 `ubuntu-latest` 上编译生成 Linux 二进制
2. 打包为 `dirleaks-<version>-linux.tar.gz`
3. 自动创建 GitHub Release 并上传产物

```bash
git tag v1.1.0
git push origin v1.1.0
```

---

## 法律合规与免责声明

- 本工具仅供**授权的渗透测试、安全研究、教育学习**使用
- 未经授权使用本工具对目标进行扫描，可能违反以下法律法规：
  - 刑事诉讼法 第138条（电子数据使用规定）
  - 网络安全法 第50条（运营者安全义务）
  - 公安机关办理刑事案件程序规定 第234条
  - 电子签名法 第14条（电子签名效力）
  - 电子数据取证规范 GA/T 1070-2013
- 使用者须自行承担一切法律与道德责任，作者不对任何非法使用行为负责

---

## 开源赞助

感谢使用本项目。您的支持是开源持续发展的核心动力。

### 国内用户

| 支付宝 | 微信支付 |
|--------|----------|
| <img src="https://github.com/ctkqiang/ctkqiang/blob/main/assets/IMG_9863.jpg?raw=true" width="160" /> | <img src="https://github.com/ctkqiang/ctkqiang/blob/main/assets/IMG_9859.JPG?raw=true" width="160" /> |

### 国际用户

[![Alipay](https://img.shields.io/badge/Alipay-全球支付-00A1E9?style=flat-square&logo=alipay&logoColor=white&labelColor=008CD7)](https://qr.alipay.com/fkx19369scgxdrkv8mxso92)
[![Ko-fi](https://img.shields.io/badge/Ko--fi-买杯咖啡-FF5E5B?style=flat-square&logo=ko-fi&logoColor=white)](https://ko-fi.com/F1F5VCZJU)
[![PayPal](https://img.shields.io/badge/PayPal-安全支付-00457C?style=flat-square&logo=paypal&logoColor=white)](https://www.paypal.com/paypalme/ctkqiang)
[![Stripe](https://img.shields.io/badge/Stripe-企业级支付-626CD9?style=flat-square&logo=stripe&logoColor=white)](https://donate.stripe.com/00gg2nefu6TK1LqeUY)

---

## 开发者

[![GitHub](https://img.shields.io/badge/GitHub-ctkqiang-181717?style=for-the-badge&logo=github)](https://github.com/ctkqiang)
[![Stack Overflow](https://img.shields.io/badge/Stack_Overflow-技术问答-F58025?style=for-the-badge&logo=stackoverflow)](https://stackoverflow.com/users/10758321/%e9%92%9f%e6%99%ba%e5%bc%ba)
[![LinkedIn](https://img.shields.io/badge/LinkedIn-职业网络-0A66C2?style=for-the-badge&logo=linkedin)](https://www.linkedin.com/in/ctkqiang/)
[![Instagram](https://img.shields.io/badge/Instagram-生活瞬间-E4405F?style=for-the-badge&logo=instagram)](https://www.instagram.com/ctkqiang)
[![Twitch](https://img.shields.io/badge/Twitch-技术直播-9146FF?style=for-the-badge&logo=twitch)](https://twitch.tv/ctkqiang)
[![WeChat](https://img.shields.io/badge/微信公众号-钟智强-07C160?style=for-the-badge&logo=wechat)](https://github.com/ctkqiang/ctkqiang/blob/main/assets/IMG_9245.JPG?raw=true)
