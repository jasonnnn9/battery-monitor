# ESP-01S / Thinger.io

STM32 独立采样和保护，通过 PA9/TX 向 ESP RX 发送测量快照；ESP 使用 Wi-Fi 连接 Thinger.io。云端仅提供读取资源，不控制继电器或保护阈值。两端均为 115200、8N1、共地，绿板已有串口连接。STM32 的 `ESP8266_THINGER_BRIDGE=1` 使用此模式，不能与旧手机直连 AT 固件混用。

## 串口协议

每约 2 秒发送一组数据，所有页面均执行。实际采样和发送间隔仍受前台任务影响。第一行兼容早期温度测试固件，新 ESP 固件忽略该行：

```text
BMS,TEMP,27
BMS,V1,0,4240,1,27,100,0,0
```

第二行依次为序号、电压 mV、电流 mA、温度 °C、电量估算百分比、报警位和负载开关状态。电流无效时为 -1，报警位 bit3 同时置位。其余报警位 bit0 欠压、bit1 过流、bit2 过温。电流目前取绝对值，电量沿用电压换算，均不能视为已完成计量校准的结果。功率为电压与电流绝对值的乘积。

ESP 丢弃超长、格式错误、越界或状态不一致的数据帧。`sample_age_ms` 是 ESP 接收后经过的时间；5 秒无有效帧后 `data_valid=false`，不再输出旧测量值。仪表盘必须使用有效标记，不能把断线当成测量值为零。

## 编译

使用 Arduino CLI、ESP8266 Arduino Core 3.1.2、thinger.io 库 2.49.0。当前机器已有工具；其他机器需要分别准备。不要开启串口调试输出，以免污染与 STM32 的通信。

`build.ps1` 接受 `-ArduinoCli`、`-ArduinoData`、`-ArduinoUser` 三个路径参数。`-CheckOnly` 只验证编译，使用占位凭据，生成的文件不能烧录。

Windows 下工具目录较长时，脚本会临时映射一个空闲盘符供旧版 ESP GCC 使用，并在结束时移除映射，不迁移或重装工具。协议检查可在项目根目录运行：

```powershell
arm-none-eabi-g++ -std=c++14 -Wall -Wextra -Werror -fsyntax-only iot/esp01s/tests/telemetry_test.cpp
```

测试使用编译期断言验证实际解析函数，包含字段映射、整数溢出、缺失/多余字段、无效电流标记及报警位一致性。

实际编译从当前 PowerShell 进程的环境变量读取 `THINGER_USERNAME`、`THINGER_DEVICE_ID`、`THINGER_DEVICE_CREDENTIAL`、`BMS_WIFI_SSID`、`BMS_WIFI_PASSWORD`。也可以加 `-Configure` 交互输入，脚本将其放入进程环境变量；密钥和密码不回显。生成的本机密钥头文件和 build 目录已忽略，不能上传固件 BIN 或密钥头文件到公开仓库。

## 烧录与验证

先验证 ESP 芯片身份并备份其现有 Flash。只有串口连接和供电稳定后才写入 production 固件。不要烧录 check 目录的占位固件。ESP 更新需要可靠的 3.3 V 供电、USB 转串口及正确的 GPIO0 启动状态；ST-LINK 只负责 STM32，不能直接烧录 ESP。此文档不代表烧录或云端验收已经完成。

更新后将 ESP 装回绿板，在 Thinger.io 的设备 API 中运行：

- `status`：固件版本应为 `bms-telemetry-v1`，有效帧计数应持续增加。
- `telemetry`：`data_valid=true`，显示 `voltage_v`、`current_ma`、`power_w`、`temperature_c`、`capacity_percent`、`warning_flags`、`load_enabled`、序号和数据年龄。电流无效时省略电流和功率，并给出 `current_valid=false`。

先与 OLED 比较，再切换设置页面检查序号继续增加；断开网络时验证本地监测和保护继续工作。Device API 可读后再将仪表盘数据源绑定到这些字段。此版本没有自动写入历史 Data Bucket，历史存储间隔需按账户限制配置。

参考：[Thinger.io ESP8266](https://docs.thinger.io/devices/arduino/boards/esp8266)、[设备资源与调用](https://docs.thinger.io/devices/arduino/coding)。
