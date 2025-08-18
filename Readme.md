# 渗透测试敏感路径扫描工具（dirleaks）

## 项目简介

**dirleaks** 是一款轻量级、高效的敏感路径扫描工具，专为 **渗透测试人员、红队、安全研究员** 设计。
它能够快速识别目标站点中常见的敏感文件、配置文件、备份文件和目录泄露问题，帮助安全从业者在信息收集阶段迅速发现潜在的攻击面。

本项目基于 **C 语言 + libcurl** 实现，保证了跨平台兼容性与性能，提供交互式终端菜单，简单易用，适合从个人测试到团队批量渗透任务。

---

## 功能特性

* **多语言支持**：内置多种常见开发语言与运行环境的路径字典

  * C# / Docker / Java / PHP / Python / Linux / Windows / 通用路径
* **批量扫描**：可一次性对指定 URL 进行全量字典探测
* **快速检测**：基于 libcurl 实现高效 HTTP 请求与状态码解析
* **分类结果展示**：结果分为 `[找到] / [未找到] / [错误]`，清晰直观
* **日志功能**：

  * 支持开启全局日志模式，将完整扫描结果保存到 `out/目标名.log`
  * 日志文件包含所有状态详情，便于后续分析与归档
* **交互式菜单**：一键选择目标类型，无需复杂命令参数

---

## 使用方法

### 编译项目

确保已安装 GCC 与 libcurl 开发包：

```bash
make
```

### 运行工具

```bash
./dirleaks
```

运行后进入交互式菜单：

```
=== 渗透工具向导菜单 ===
0) 扫描所有路径
1) C#
2) Docker
3) 通用路径
4) Java
5) Linux
6) PHP
7) Python
8) Windows
请选择 [0-8]:
```

输入目标 URL（如 `http://127.0.0.1:8080`）即可自动开始扫描。

### 日志输出

默认在终端展示结果。若在 `dirleaks.c` 中定义全局宏：

```c
#define ENABLE_LOG 1
```

则扫描结果会自动写入：

```
out/<目标域名>.log
```

日志中包含所有请求路径、状态码与分类，便于后续追踪与分享。

---

## 结果展示示例

```text
--- 扫描 PHP ---
[x] /.env                     [找到] 状态: 200
[ ] /vendor/                  [未找到] 状态: 403
[ ] /app/etc/env.php          [错误]
```

说明：

* `[找到]`：目标返回状态码为 200，存在敏感路径
* `[未找到]`：请求成功，但路径不存在（通常 403/404）
* `[错误]`：请求过程中出现超时或网络异常

---

## 目录结构

```
├── dirleaks.c         # 主程序入口
├── include/           # 头文件目录
│   ├── csharp_path.h
│   ├── docker_path.h
│   ├── generic_path.h
│   ├── http.h
│   ├── java_path.h
│   ├── linux_path.h
│   ├── php_path.h
│   ├── python_path.h
│   ├── scanner.h
│   ├── tui.h
│   └── windows_path.h
├── src/               # 源码目录
│   ├── csharp_path.c
│   ├── docker_path.c
│   ├── generic_path.c
│   ├── http.c
│   ├── java_path.c
│   ├── linux_path.c
│   ├── php_path.c
│   ├── python_path.c
│   ├── scanner.c
│   ├── tui.c
│   └── windows_path.c
├── makefile           # 编译脚本
├── Readme.md          # 项目说明文档
└── out/               # 扫描日志输出目录
```

---

## 依赖环境

* GCC 编译器
* libcurl 开发库

  * macOS: `brew install curl`
  * Debian/Ubuntu: `sudo apt-get install libcurl4-openssl-dev`
  * CentOS/RHEL: `sudo yum install libcurl-devel`

---

## 法律合规与免责声明

* 本工具仅供 **授权的渗透测试、安全研究、教育学习** 使用
* 未经授权使用本工具对目标进行扫描，可能违反相关法律法规
* 使用者须自行承担一切法律与道德责任
* 作者不对任何非法使用行为负责

---

## 未来规划

* [ ] 增加多线程扫描，提升大规模目标效率
* [ ] 增加 YAML/JSON 格式导出
* [ ] 内置绕过机制（大小写混淆、路径变形）
* [ ] 在线字典更新支持
* [ ] 更丰富的 TUI 交互（进度条、统计信息）

---

