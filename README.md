# Battery Monitor

基于 STM32F103C8T6 的电池监测教学项目，使用 OLED、INA219、DS18B20、继电器和 ESP8266。当前开发环境为 64 位 Windows、VS Code、Arm GCC 和 OpenOCD。

## 获取工程

~~~powershell
git clone https://github.com/jasonnnn9/battery-monitor.git
cd battery-monitor
code .
~~~

仓库公开，可通过链接查看、下载或克隆。直接推送分支需要接受协作者邀请，并使用自己的 GitHub 账号认证。

## 工具安装

在本仓库的上一级建立 .bms-debug-tools，下载并解压以下两个固定版本。将解压得到的版本目录直接放入 .bms-debug-tools。

- [Arm GCC 15.2.1-1.1，Windows x64](https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack/releases/download/v15.2.1-1.1/xpack-arm-none-eabi-gcc-15.2.1-1.1-win32-x64.zip)
- [OpenOCD 0.12.0-7，Windows x64](https://github.com/xpack-dev-tools/openocd-xpack/releases/download/v0.12.0-7/xpack-openocd-0.12.0-7-win32-x64.zip)

目录应为下面的结构。

~~~text
工作目录/
├── battery-monitor/
│   ├── .vscode/
│   ├── Core/
│   ├── Drivers/
│   ├── Hardware/
│   ├── Show/
│   └── system/
└── .bms-debug-tools/
    ├── xpack-arm-none-eabi-gcc-15.2.1-1.1/
    │   └── bin/arm-none-eabi-gcc.exe
    └── xpack-openocd-0.12.0-7/
        └── bin/openocd.exe
~~~

在 VS Code 安装 Microsoft C/C++ 扩展。断点调试还需安装 Cortex-Debug（marus25.cortex-debug）。dfu-util 已随工程提供。使用硬件时，Windows 需能够识别对应的 ST-LINK 或 Maple DFU 设备。

## 编译与烧录

用 VS Code 打开仓库根目录，按 Ctrl+Shift+B。也可以在仓库根目录执行下面的命令。

~~~powershell
powershell.exe -NoProfile -ExecutionPolicy RemoteSigned -File .\.vscode\build-gcc.ps1
~~~

输出位于 build-gcc，包含 BIN、ELF 和 MAP。应用链接地址为 0x08002000。生成文件已列入 Git 忽略规则。

在终端菜单的运行任务中选择相应任务。

| 任务 | 使用方式 |
|---|---|
| Upload firmware via ST-LINK (OpenOCD) | 蓝板装在绿板上时可用。BOOT0=0、BOOT1=0，接 ST-LINK 和板子电源。 |
| Upload firmware via Micro-USB (DFU) | 已安装 Maple 的蓝板单独接 Micro-USB。BOOT0=0、BOOT1=1，按 RESET，确认设备 1EAF:0003。上传后将 BOOT1 恢复为 0 再复位。 |
| Install USB bootloader once (ST-LINK) | 新板首次安装 Maple 和当前应用。先编译，再接 ST-LINK。任务先读出该板 Flash 至本机 %LOCALAPPDATA%\BatteryMonitor\flash-backups，再写入引导程序和应用。 |

当前硬件上已验证蓝板单独连接时 USB DFU 可用，蓝板装在绿板上时使用 ST-LINK。进入 BMS 主界面需按绿板页面键。

## 源码入口

| 文件 | 内容 |
|---|---|
| Core/Src/main.c | 初始化顺序与主循环 |
| system/keypad/keypad.c | 按键、OLED 页面、阈值和报警动作 |
| Hardware/esp8266/esp8266.c | 热点、TCP 服务和手机指令 |
| system/Adc/adc.c | PB1 电池分压采样（5∶1，ADC 参考电压按 3.3 V 换算） |
| Hardware/Ina219/Ina219.c | R100 分流电阻（0.1 Ω）电流采样及校准 |
| Hardware/18B20/ds18b20.c | 温度传感器 |
| Show/SD1306/SD1306.c | OLED 驱动 |
| system/init/init.h | 继电器、蜂鸣器等引脚 |
| .vscode/firmware-sources.psd1 | 编译源文件、宏定义和头文件路径 |

新增 C 文件时同步更新 firmware-sources.psd1。调试配置使用 build-gcc/Demo_f103C8T6.elf，调试前应烧录与该 ELF 对应的固件。

主界面和手机端显示电流的绝对值（mA）。电流读数无效时，OLED 显示 `ERR`，手机数据中的电流为 `-1`，并设置 `warningFlag` 的第 3 位（值 8）。此状态下负载继电器关断，读数恢复后可手动重新接通负载。

ESP8266 提供 Wi-Fi 热点与 8080 端口 TCP 服务，手机端 APK 单独分发。测量更新与保护判断在所有页面持续执行，完整的电池充放电保护尚需专项验收。

## 组员协作

每个功能使用独立分支，修改前更新主分支。

~~~powershell
git switch main
git pull --ff-only
git switch -c feature/your-feature
~~~

修改后编译并检查差异，选择本次改动的文件提交。

~~~powershell
git diff
git add Core/Src/main.c
git commit -m "Describe the change"
git push -u origin feature/your-feature
~~~

在 GitHub 创建 Pull Request，说明改动、构建结果和实板测试情况。main 要求任意一位有写权限的成员批准，并处理完所有审查讨论，才能合并。审核人不限于特定成员，PR 作者不能批准自己的 PR。新增提交后，先前的批准会失效，需要重新审查。main 同时禁止强制推送和删除，管理员也适用这些规则。Issues 可用于登记功能分工和故障。

## 项目设计与开发

本项目由小组完成电路设计及应用代码的修改、开发与验证。小组负责系统功能设计、硬件联调，以及电池测量、显示、按键、保护和 Wi-Fi 通信等应用功能的维护。

工程采用 ST HAL、CMSIS、dfu-util 等第三方组件，相关组件的版权声明和许可按各自文件保留。Maple 引导镜像来自 [STM32duino-bootloader](https://github.com/rogerclarkmelbourne/STM32duino-bootloader)。应用源码中保留的原工程署名用于记录代码来源。

当前已记录的工程改动包括 GCC 迁移、ESP8266 启动命令修复、OLED 就绪等待、源码整理和裸机链接配置修正。构建验证与实板验证应在 Pull Request 中分别记录。

## 当前测试版操作

设置页中 S2 减小、S3 增大。主页面的 S3 保留手动切换负载的功能，异常时禁止接通负载。

V9 绿板 D1 指示灯与 BUZZER1 共用 VT1 驱动，由 PA11 控制。PCB 网络中 BUZZER1 的正端和 R1 的一端同接 VT1_1，D1 负端与 BUZZER1 负端接地；PB9 没有连接这盏灯。因此当前硬件无法独立实现正常时灯亮且蜂鸣器静音。正常时两者关闭，欠压、过流或过温时同步报警。电流传感器故障仍关断负载，并显示 ERR。设置页期间继续更新测量和保护。

当前通过 system/init/init.h 中的 BEEP_ENABLED=1 启用声光报警。欠压、过流和过温采用固定 500 ms 周期，每周期响/亮 200 ms、停/灭 300 ms，由 TIM2 更新，不随页面刷新或串口等待改变节奏，也不再通过报警延时阻塞前台采样。故障解除后关闭报警。手机命令的单次 200 ms 反馈也由 TIM2 执行，持续故障报警优先。静音测试时可设为 0，但 V9 共用驱动的指示灯也会关闭。时序精度受定时中断调度影响。

电量显示使用有效图标索引和固定宽度百分比，修复满电图标越界与百分号重叠。电量百分比目前仍使用原有电压换算，准确性需另行校准。

### 设置页期间的监测

每轮主循环先采集电压、电流和温度，再执行保护判断，之后处理按键和显示。空白页、主页面和三个设置页均执行采样与保护。

进入某个设置页时复制当前生效阈值。S2、S3 编辑屏幕临时值，保护继续使用原阈值；按 S1 离开该设置页时提交这一项新值，并立即根据最近读数重新判断报警。手机设置仍直接修改生效阈值，通信处理保持在主页面。

采样在前台主循环中执行，实际间隔受现有按键消抖、温度读取和串口等待影响。此版本没有提供固定的保护响应时间保证。

验收时可在主页面设定温度阈值，进入电压或电流设置页后改变探头温度，观察故障灯与继电器动作。编辑温度临时值时，原阈值应持续生效；按 S1 离开温度页后，新值应生效。

### Thinger.io 云端监测

云端模式代码在 [iot/esp01s](iot/esp01s/README.md)。STM32 使用 `ESP8266_THINGER_BRIDGE=1`，通过串口每约 2 秒发送测量快照，所有页面均继续发送；ESP-01S 需要配套的 `BatteryMonitorThinger` 固件。旧的 ESP 连通性测试程序只显示在线状态，不能提供完整测量资源。

云端提供只读 `status` 与 `telemetry` 资源，包含电压、电流、功率、温度、电量估算、报警位和负载开关状态。超过 5 秒无有效串口帧时标记数据失效；网络断开不改变 STM32 本地保护。此模式不使用 ESP 的 AT 指令或旧手机直连 App，云端不控制负载或保护阈值。

凭据由本机环境变量提供，不上传源码仓库。`build.ps1 -CheckOnly` 生成的占位凭据固件只能检查编译，不能烧录。完整端到端验收应确认有效帧计数递增、云端与 OLED 读数一致、设置页面仍更新及断网期间本地保护正常。