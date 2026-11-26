# ESP-01S / Thinger.io

STM32 独立采样、执行保护并保存阈值。PA9/TX 向 ESP RX 发送测量和配置，PA10/RX 从 ESP TX 接收修改请求。绿板已有这两根信号线，两端共地，串口为 115200、8N1。ESP 使用 Wi-Fi 连接 Thinger.io，并与启用 `ESP8266_THINGER_BRIDGE=1` 的 STM32 固件配套使用。

## 串口协议

每约 2 秒发送一组数据，所有页面均执行。实际采样和发送间隔仍受前台任务影响。第一行兼容早期温度测试固件，新 ESP 固件忽略该行：

```text
BMS,TEMP,27
BMS,V1,0,4240,1,27,100,0,0
```

第二行依次为序号、电压 mV、电流 mA、温度 °C、电量估算百分比、报警位和负载开关状态。电流无效时为 -1，报警位 bit3 同时置位。其余报警位 bit0 欠压、bit1 过流、bit2 过温。电流目前取绝对值，电量沿用电压换算，均不能视为已完成计量校准的结果。功率为电压与电流绝对值的乘积。

Warning bit 4 (value 16) indicates a temperature probe fault. In such frames the numeric temperature field is a placeholder, and `telemetry` reports `temperature_valid=false` and omits `temperature_c`. The other measurements continue to update. Deploy ESP firmware 1.1.2 before updating the STM32, since older ESP parsers reject frames containing bit 4.

On the V9 bench, use a safe test load and set the thresholds so normal readings permit the relay to close. Enable protection and the load, then disconnect the probe. Check that the OLED shows a temperature error, the relay opens, and the cloud flags the temperature as invalid. After reconnecting the probe, check that valid readings return and the load stays off until switched on locally.

ESP 丢弃超长、格式错误、越界或状态不一致的数据帧。`sample_age_ms` 是 ESP 接收后经过的时间；5 秒无有效帧后 `data_valid=false`，不再输出旧测量值。仪表盘必须使用有效标记，不能把断线当成测量值为零。

## 编译

使用 Arduino CLI、ESP8266 Arduino Core 3.1.2、thinger.io 库 2.49.0。当前机器已有工具；其他机器需要分别准备。不要开启串口调试输出，以免污染与 STM32 的通信。

`build.ps1` 接受 `-ArduinoCli`、`-ArduinoData`、`-ArduinoUser` 三个路径参数。`-CheckOnly` 只验证编译，使用占位凭据，生成的文件不能烧录。

Windows 下工具目录较长时，脚本会临时映射一个空闲盘符供旧版 ESP GCC 使用，并在结束时移除映射，不迁移或重装工具。协议检查可在项目根目录运行：

```powershell
arm-none-eabi-g++ -std=c++14 -Wall -Wextra -Werror -I Protocol -fsyntax-only iot/esp01s/tests/telemetry_test.cpp
arm-none-eabi-g++ -std=c++14 -Wall -Wextra -Werror -fsyntax-only iot/esp01s/tests/control_test.cpp
```

测试使用编译期断言验证实际解析函数，包含字段映射、整数溢出、缺失/多余字段、无效电流标记及报警位一致性。

实际编译从当前 PowerShell 进程的环境变量读取 `THINGER_USERNAME`、`THINGER_DEVICE_ID`、`THINGER_DEVICE_CREDENTIAL`、`BMS_WIFI_SSID`、`BMS_WIFI_PASSWORD`。也可以加 `-Configure` 交互输入，脚本将其放入进程环境变量；密钥和密码不回显。生成的本机密钥头文件和 build 目录已忽略，不能上传固件 BIN 或密钥头文件到公开仓库。

## 烧录与验证

先验证 ESP 芯片身份并备份其现有 Flash。只有串口连接和供电稳定后才写入 production 固件。不要烧录 check 目录的占位固件。ESP 更新需要可靠的 3.3 V 供电、USB 转串口及正确的 GPIO0 启动状态；ST-LINK 只负责 STM32，不能直接烧录 ESP。此文档不代表烧录或云端验收已经完成。

更新后将 ESP 装回绿板，在 Thinger.io 的设备 API 中运行：

- `status` shows firmware `bms-control-v1.1.2-ota`, OTA availability and free space. The valid-frame counter should keep increasing with both boards installed.
- `telemetry`：`data_valid=true`，显示 `voltage_v`、`current_ma`、`power_w`、`temperature_c`、`capacity_percent`、`warning_flags`、`load_enabled`、序号和数据年龄。电流无效时省略电流和功率，并给出 `current_valid=false`。

先与 OLED 比较，再切换设置页面检查序号继续增加；断开网络时验证本地监测和保护继续工作。Device API 可读后再将仪表盘数据源绑定到这些字段。此版本没有自动写入历史 Data Bucket，历史存储间隔需按账户限制配置。

## 读取与修改保护阈值

在 Device API 中打开 `protection`，空输入读取当前配置。`board_online=true` 表示 ESP 最近 5 秒内收到过 STM32 的测量和配置帧。写入时同时提供三个数值，单位与字段名一致。以下示例使用项目初始值，范围是协议允许值，具体电池与负载的阈值应按其规格选取。

```json
{"low_voltage_v": 2.5, "high_current_ma": 180, "high_temperature_c": 35}
```

欠压范围 0.5–16.5 V，步长 0.1 V。过流为 10–3200 mA 的整数，过温为 0–99 °C 的整数。STM32 用带 CRC 的 SET 和 CFG 帧接收请求、回报实际值。若返回 `pending`，稍后空输入重读，核对同一个 `request_id` 的 `applied` 与三个实际值。若为 `timeout`，先重读实际配置。`local_editing` 表示有人正在板上设置，`configuration_changed` 表示提交前配置已变化，均应读取最新值再操作。保存失败时保留上一份生效配置。验收时可在电池和负载断开的情况下保存一组测试值，整板断电再上电，对照 OLED 和云端读数，最后恢复原设置。

## ESP 无线更新

首次经串口烧入带 OTA 的正式固件，然后让 ESP 正常启动并连入 Thinger.io。在 VS Code 中把本目录作为 PlatformIO 项目打开，选择 `esp01s` 环境，使用 Thinger.io 扩展选定目标设备并更新。扩展需要账户访问令牌，权限见官方说明。沿用本机的五项配置或已有正式构建的私有配置。每次发布递增 `THINGER_OTA_VERSION`。本项目给 1 MB Flash 设置了 468 KiB 镜像上限，更新前还应核对设备 `status.ota_free_bytes`。完成后查看设备重新上线和版本变化，再核对遥测与阈值。此路径仅更新 ESP，STM32 应用仍单独烧录。[Thinger.io OTA 操作说明](https://docs.thinger.io/devices/ota)

参考：[Thinger.io ESP8266](https://docs.thinger.io/devices/arduino/boards/esp8266)、[设备资源与调用](https://docs.thinger.io/devices/arduino/coding)。
