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


## 🌟 开源项目赞助计划

### 用捐赠助力发展

感谢您使用本项目！您的支持是开源持续发展的核心动力。  
每一份捐赠都将直接用于：  
✅ 服务器与基础设施维护（魔法城堡的维修费哟~）  
✅ 新功能开发与版本迭代（魔法技能树要升级哒~）  
✅ 文档优化与社区建设（魔法图书馆要扩建呀~）

点滴支持皆能汇聚成海，让我们共同打造更强大的开源工具！  
（小仙子们在向你比心哟~）

---

### 🌐 全球捐赠通道

#### 国内用户

<div align="center" style="margin: 40px 0">

<div align="center">
<table>
<tr>
<td align="center" width="300">
<img src="https://github.com/ctkqiang/ctkqiang/blob/main/assets/IMG_9863.jpg?raw=true" width="200" />
<br />
<strong>🔵 支付宝</strong>（小企鹅在收金币哟~）
</td>
<td align="center" width="300">
<img src="https://github.com/ctkqiang/ctkqiang/blob/main/assets/IMG_9859.JPG?raw=true" width="200" />
<br />
<strong>🟢 微信支付</strong>（小绿龙在收金币哟~）
</td>
</tr>
</table>
</div>
</div>

#### 国际用户

<div align="center" style="margin: 40px 0">
  <a href="https://qr.alipay.com/fkx19369scgxdrkv8mxso92" target="_blank">
    <img src="https://img.shields.io/badge/Alipay-全球支付-00A1E9?style=flat-square&logo=alipay&logoColor=white&labelColor=008CD7">
  </a>
  
  <a href="https://ko-fi.com/F1F5VCZJU" target="_blank">
    <img src="https://img.shields.io/badge/Ko--fi-买杯咖啡-FF5E5B?style=flat-square&logo=ko-fi&logoColor=white">
  </a>
  
  <a href="https://www.paypal.com/paypalme/ctkqiang" target="_blank">
    <img src="https://img.shields.io/badge/PayPal-安全支付-00457C?style=flat-square&logo=paypal&logoColor=white">
  </a>
  
  <a href="https://donate.stripe.com/00gg2nefu6TK1LqeUY" target="_blank">
    <img src="https://img.shields.io/badge/Stripe-企业级支付-626CD9?style=flat-square&logo=stripe&logoColor=white">
  </a>
</div>

---

### 📌 开发者社交图谱

#### 技术交流

<div align="center" style="margin: 20px 0">
  <a href="https://github.com/ctkqiang" target="_blank">
    <img src="https://img.shields.io/badge/GitHub-开源仓库-181717?style=for-the-badge&logo=github">
  </a>
  
  <a href="https://stackoverflow.com/users/10758321/%e9%92%9f%e6%99%ba%e5%bc%ba" target="_blank">
    <img src="https://img.shields.io/badge/Stack_Overflow-技术问答-F58025?style=for-the-badge&logo=stackoverflow">
  </a>
  
  <a href="https://www.linkedin.com/in/ctkqiang/" target="_blank">
    <img src="https://img.shields.io/badge/LinkedIn-职业网络-0A66C2?style=for-the-badge&logo=linkedin">
  </a>
</div>

#### 社交互动

<div align="center" style="margin: 20px 0">
  <a href="https://www.instagram.com/ctkqiang" target="_blank">
    <img src="https://img.shields.io/badge/Instagram-生活瞬间-E4405F?style=for-the-badge&logo=instagram">
  </a>
  
  <a href="https://twitch.tv/ctkqiang" target="_blank">
    <img src="https://img.shields.io/badge/Twitch-技术直播-9146FF?style=for-the-badge&logo=twitch">
  </a>
  
  <a href="https://github.com/ctkqiang/ctkqiang/blob/main/assets/IMG_9245.JPG?raw=true" target="_blank">
    <img src="https://img.shields.io/badge/微信公众号-钟智强-07C160?style=for-the-badge&logo=wechat">
  </a>
</div>

---

🙌 感谢您成为开源社区的重要一员！  
💬 捐赠后欢迎通过社交平台与我联系，您的名字将出现在项目致谢列表！