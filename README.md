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

ESP8266 提供 Wi-Fi 热点与 8080 端口 TCP 服务，手机端 APK 单独分发。当前保护判断及部分测量更新依赖主页面，完整的电池充放电保护尚需专项验收。

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

在 GitHub 创建 Pull Request，说明改动、构建结果和实板测试情况，由另一位组员检查后合并。Issues 可用于登记功能分工和故障。

## 项目设计与开发

本项目由小组完成电路设计及应用代码的修改、开发与验证。小组负责系统功能设计、硬件联调，以及电池测量、显示、按键、保护和 Wi-Fi 通信等应用功能的维护。

工程采用 ST HAL、CMSIS、dfu-util 等第三方组件，相关组件的版权声明和许可按各自文件保留。Maple 引导镜像来自 [STM32duino-bootloader](https://github.com/rogerclarkmelbourne/STM32duino-bootloader)。应用源码中保留的原工程署名用于记录代码来源。

当前已记录的工程改动包括 GCC 迁移、ESP8266 启动命令修复、OLED 就绪等待、源码整理和裸机链接配置修正。构建验证与实板验证应在 Pull Request 中分别记录。

## 当前测试版操作

设置页中 S2 减小、S3 增大。主页面的 S3 保留手动切换负载的功能，异常时禁止接通负载。

绿板 LED 表示系统状态。正常时常亮，欠压、过流、过温或电流传感器故障时每 300 ms 切换亮灭。LED 由现有 TIM2 定时回调更新。当前报警判断在主页面刷新，停在设置页时按上次的报警状态显示。

测试版通过 system/init/init.h 中的 BEEP_ENABLED=0 关闭蜂鸣器反馈及鸣叫延时。恢复声音时将其设为 1 后重新编译烧录。

电量显示使用有效图标索引和固定宽度百分比，修复满电图标越界与百分号重叠。电量百分比目前仍使用原有电压换算，准确性需另行校准。
