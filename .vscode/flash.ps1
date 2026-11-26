param([Parameter(Mandatory)][ValidateSet('Install','Upload','UploadSwd')][string]$Mode)
$ErrorActionPreference = 'Stop'
$root = Split-Path $PSScriptRoot
$app = Join-Path $root 'build-gcc\Demo_f103C8T6-usb.bin'
$data = [IO.File]::ReadAllBytes($app)
# The last 2 KiB hold the STM32 protection settings.
if ($data.Length -lt 8 -or $data.Length -gt 0xD800) { throw 'Expected an application of 8 bytes to 54 KiB.' }
$reset = [BitConverter]::ToUInt32($data, 4)
if ($reset -lt 0x08002000 -or $reset -ge (0x08002000 + $data.Length) -or !($reset -band 1)) {
    throw 'Expected the application linked at 0x08002000.'
}
if ($Mode -eq 'Upload') {
    $dfu = Join-Path $PSScriptRoot 'dfu-util\dfu-util-0.11-win64\dfu-util.exe'
    Write-Host 'Waiting for Maple DFU. Set BOOT0=0 and BOOT1=1, then press RESET. Ctrl+C cancels.'
    & $dfu -w -d 1EAF:0003 -a 2 -D $app
    if ($LASTEXITCODE -ne 0) { throw 'DFU upload failed. Check the Maple DFU driver and the output above.' }
    return
}

$openocd = Join-Path $root '../.bms-debug-tools/xpack-openocd-0.12.0-7/bin/openocd.exe'
$commands = @('init', 'reset halt')
if ($Mode -eq 'Install') {
    $boot = Join-Path $root 'generic_boot20_pc13.bin'
    $bootData = [IO.File]::ReadAllBytes($boot)
    if ($bootData.Length -lt 8 -or $bootData.Length -gt 0x2000) { throw 'Expected a bootloader-only binary of at most 8 KB.' }
    $backupDir = Join-Path $env:LOCALAPPDATA 'BatteryMonitor\flash-backups'
    New-Item -ItemType Directory -Force -Path $backupDir | Out-Null
    $backup = (Join-Path $backupDir ('before-maple-' + [guid]::NewGuid() + '.bin')).Replace('\', '/')
    $backupCommands = "init; reset halt; flash read_bank 0 {$backup}; resume; shutdown"
    & $openocd -f interface/stlink.cfg -c 'transport select swd' -f target/stm32f1x.cfg -c 'adapter speed 950' -c $backupCommands
    if ($LASTEXITCODE -ne 0 -or !(Test-Path -LiteralPath $backup) -or (Get-Item -LiteralPath $backup).Length -lt 65536) {
        throw 'Flash backup failed. Bootloader programming was not started.'
    }
    Write-Output "Original Flash backup: $backup"
    $boot = $boot.Replace('\', '/')
    $commands += "flash write_image erase {$boot} 0x08000000 bin", "verify_image {$boot} 0x08000000 bin"
}
$app = $app.Replace('\', '/')
$commands += "flash write_image erase {$app} 0x08002000 bin", "verify_image {$app} 0x08002000 bin", 'reset run', 'shutdown'
& $openocd -f interface/stlink.cfg -c 'transport select swd' -f target/stm32f1x.cfg -c 'adapter speed 950' -c ($commands -join '; ')
if ($LASTEXITCODE -ne 0) { throw 'OpenOCD programming or verification failed.' }
Write-Output 'Programming and verification completed.'

