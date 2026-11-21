"""Private build configuration shared with the Arduino CLI workflow."""
import os
import pathlib
import shutil

Import("env")  # Provided by PlatformIO/SCons.
project = pathlib.Path(env.subst("$PROJECT_DIR"))
generated = pathlib.Path(env.subst("$BUILD_DIR")) / "bms-config"
generated.mkdir(parents=True, exist_ok=True)
shutil.copyfile(project.parent.parent / "Protocol/bms_control.h", generated / "bms_control.h")
names = ("THINGER_USERNAME", "THINGER_DEVICE_ID", "THINGER_DEVICE_CREDENTIAL", "BMS_WIFI_SSID", "BMS_WIFI_PASSWORD")
destination = generated / "bms_runtime_secrets.h"
if all(os.environ.get(name) for name in names):
    lines = ["#pragma once"]
    for name in names:
        value = os.environ[name]
        if "\r" in value or "\n" in value:
            raise RuntimeError("Invalid newline in " + name)
        escaped = value.replace("\\", "\\\\").replace('"', '\\"')
        lines.append('#define %s "%s"' % (name, escaped))
    destination.write_text("\n".join(lines) + "\n", encoding="utf-8")
else:
    candidates = (
        project / "BatteryMonitorThinger/arduino_secrets.h",
        project / "build/production/BatteryMonitorThinger/arduino_secrets.h",
    )
    for candidate in candidates:
        if candidate.is_file():
            content = candidate.read_text(encoding="utf-8-sig")
            if "BUILD_CHECK_ONLY" in content or "YOUR_DEVICE_CREDENTIAL" in content:
                raise RuntimeError("Configure real local credentials before a production build.")
            destination.write_text(content, encoding="utf-8")
            break
    else:
        raise RuntimeError("Provide the five credential environment variables or run build.ps1 -Configure.")
env.Prepend(CPPPATH=[str(generated)])


def check_ota_budget(source, target, env):
    image = pathlib.Path(env.subst("$BUILD_DIR/${PROGNAME}.bin"))
    if image.stat().st_size > 0x75000:
        raise RuntimeError("Firmware exceeds the 468 KiB OTA image budget.")


env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", check_ota_budget)
