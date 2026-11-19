param(
    [Parameter(Mandatory = $true)][string]$ArduinoCli,
    [Parameter(Mandatory = $true)][string]$ArduinoData,
    [Parameter(Mandatory = $true)][string]$ArduinoUser,
    [switch]$CheckOnly,
    [switch]$Configure
)
$ErrorActionPreference = 'Stop'

if ($Configure) {
    $env:THINGER_USERNAME = Read-Host 'Thinger.io username'
    $env:THINGER_DEVICE_ID = Read-Host 'Device ID (bms_esp01s)'
    $env:BMS_WIFI_SSID = Read-Host '2.4 GHz Wi-Fi SSID'
    foreach ($name in @('THINGER_DEVICE_CREDENTIAL', 'BMS_WIFI_PASSWORD')) {
        $secure = Read-Host $name -AsSecureString
        $pointer = [Runtime.InteropServices.Marshal]::SecureStringToBSTR($secure)
        try {
            [Environment]::SetEnvironmentVariable($name,
                [Runtime.InteropServices.Marshal]::PtrToStringBSTR($pointer), 'Process')
        } finally {
            [Runtime.InteropServices.Marshal]::ZeroFreeBSTR($pointer)
        }
    }
}

$profile = if ($CheckOnly) { 'check' } else { 'production' }
$output = Join-Path $PSScriptRoot "build/$profile"
$sketch = Join-Path $output 'BatteryMonitorThinger'
New-Item -ItemType Directory -Force -Path $sketch | Out-Null
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'BatteryMonitorThinger/BatteryMonitorThinger.ino') -Destination $sketch
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'BatteryMonitorThinger/telemetry.h') -Destination $sketch

$names = @('THINGER_USERNAME', 'THINGER_DEVICE_ID', 'THINGER_DEVICE_CREDENTIAL', 'BMS_WIFI_SSID', 'BMS_WIFI_PASSWORD')
$header = @('#pragma once')
foreach ($name in $names) {
    $value = if ($CheckOnly) { 'BUILD_CHECK_ONLY' } else { [Environment]::GetEnvironmentVariable($name, 'Process') }
    if ([string]::IsNullOrEmpty($value)) { throw "Missing environment variable: $name" }
    if ($value.Contains("`r") -or $value.Contains("`n")) { throw "Invalid newline in $name" }
    $escaped = $value.Replace('\', '\\').Replace('"', '\"')
    $header += '#define ' + $name + ' "' + $escaped + '"'
}
[IO.File]::WriteAllLines((Join-Path $sketch 'arduino_secrets.h'), $header, (New-Object Text.UTF8Encoding $false))
$oldData = $env:ARDUINO_DIRECTORIES_DATA
$oldUser = $env:ARDUINO_DIRECTORIES_USER
$mappedDrive = $null
try {
    $env:ARDUINO_DIRECTORIES_DATA = $ArduinoData
    $env:ARDUINO_DIRECTORIES_USER = $ArduinoUser
    # Older ESP GCC expands ../ paths beyond Windows MAX_PATH before opening headers.
    if ($env:OS -eq 'Windows_NT' -and $ArduinoData.Length -gt 40) {
        $usedDrives = @(Get-PSDrive -PSProvider FileSystem | ForEach-Object { $_.Name })
        foreach ($letter in @('R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z')) {
            if ($letter -notin $usedDrives) {
                $candidate = $letter + ':'
                & subst.exe $candidate $ArduinoData
                if ($LASTEXITCODE -ne 0) { throw 'Could not create a temporary short toolchain path.' }
                $mappedDrive = $candidate
                $env:ARDUINO_DIRECTORIES_DATA = $candidate + '\'
                break
            }
        }
        if (!$mappedDrive) { throw 'No free drive letter for the temporary toolchain path.' }
    }
    & $ArduinoCli compile --fqbn 'esp8266:esp8266:generic:eesz=1M64,xtal=80' --build-path (Join-Path $output 'objects') --output-dir $output $sketch
    if ($LASTEXITCODE -ne 0) { throw 'ESP8266 build failed.' }
} finally {
    $env:ARDUINO_DIRECTORIES_DATA = $oldData
    $env:ARDUINO_DIRECTORIES_USER = $oldUser
    if ($mappedDrive) { & subst.exe $mappedDrive /D }
}
if ($CheckOnly) { Write-Output 'Build check passed. This image has placeholder credentials; do not flash it.' }
else { Write-Output "ESP firmware: $output/BatteryMonitorThinger.ino.bin" }
