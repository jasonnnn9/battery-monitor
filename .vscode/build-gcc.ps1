$ErrorActionPreference = 'Stop'
$root = Split-Path $PSScriptRoot
$bin = Join-Path $root '../.bms-debug-tools/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin'
$gcc = Join-Path $bin 'arm-none-eabi-gcc.exe'
$objcopy = Join-Path $bin 'arm-none-eabi-objcopy.exe'
$output = Join-Path $root 'build-gcc'
New-Item -ItemType Directory -Force -Path $output | Out-Null

$manifest = Import-PowerShellDataFile -LiteralPath (Join-Path $PSScriptRoot 'firmware-sources.psd1')
$defines = $manifest.Defines | ForEach-Object { "-D$_" }
$includes = $manifest.Includes | ForEach-Object { '-I' + (Join-Path $root $_) }
$sources = $manifest.Sources | ForEach-Object { Join-Path $root $_ }
$startup = Join-Path $root 'Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s'
$linker = Join-Path $PSScriptRoot 'stm32f103-usb.ld'
$elf = Join-Path $output 'Demo_f103C8T6.elf'
$image = Join-Path $output 'Demo_f103C8T6-usb.bin'
$map = Join-Path $output 'Demo_f103C8T6.map'

& $gcc -mcpu=cortex-m3 -mthumb -std=gnu11 -Os -g3 -ffunction-sections -fdata-sections @defines @includes @sources $startup -T $linker --specs=nano.specs --specs=nosys.specs -u _printf_float "-Wl,--gc-sections,-Map=$map,--print-memory-usage" -o $elf
if ($LASTEXITCODE -ne 0) { throw 'Arm GCC build failed.' }
& $objcopy -O binary $elf $image
if ($LASTEXITCODE -ne 0) { throw 'Could not export the USB application binary.' }
Write-Output "USB application: $image"
