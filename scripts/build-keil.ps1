param(
    [string]$Uv4 = ''
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$candidates = @(
    $Uv4,
    'D:\keil5 MDK\UV4\UV4.exe',
    'D:\keil5\UV4\UV4.exe',
    'C:\Keil_v5\UV4\UV4.exe'
) | Where-Object { $_ -and (Test-Path -LiteralPath $_) }

if (-not $candidates) {
    throw 'UV4.exe not found. Pass -Uv4 <path-to-UV4.exe>.'
}

$uv4 = (Resolve-Path -LiteralPath $candidates[0]).Path
$projects = @(
    'firmware\gd32f303-main\USER\GD32F303_RTThread.uvprojx',
    'firmware\stm32f103-spray\SPRAY_RTThread.uvprojx'
)
$logDir = Join-Path $root 'build-logs'
New-Item -ItemType Directory -Force -Path $logDir | Out-Null

foreach ($projectRel in $projects) {
    $project = Join-Path $root $projectRel
    if (-not (Test-Path -LiteralPath $project)) {
        throw "Project not found: $project"
    }

    $name = [System.IO.Path]::GetFileNameWithoutExtension($project)
    $log = Join-Path $logDir "$name.log"
    Write-Host "Building $name ..."
    $process = Start-Process -FilePath $uv4 `
        -ArgumentList @('-r', $project, '-j0', '-o', $log) `
        -WorkingDirectory (Split-Path -Parent $project) `
        -Wait -PassThru -WindowStyle Hidden

    if (Test-Path -LiteralPath $log) {
        Get-Content -LiteralPath $log -Encoding UTF8
    }

    if ($process.ExitCode -ne 0) {
        throw "Build failed: $name (exit code $($process.ExitCode))"
    }
}

Write-Host 'All Keil projects built successfully.'
