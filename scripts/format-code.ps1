param(
    [string]$ClangFormat = 'clang-format'
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$formatter = Get-Command $ClangFormat -ErrorAction SilentlyContinue
if (-not $formatter) {
    throw 'clang-format not found. Install it or pass -ClangFormat <path>.'
}

$files = @()
$files += Get-ChildItem (Join-Path $root 'firmware\gd32f303-main\APP') -File -Recurse | Where-Object { $_.Extension -in '.c', '.h' }
$files += Get-ChildItem (Join-Path $root 'firmware\gd32f303-main\BOARD') -File -Recurse | Where-Object { $_.Extension -in '.c', '.h' }
$files += Get-ChildItem (Join-Path $root 'firmware\gd32f303-main\USER') -File | Where-Object { $_.Name -in 'main.c', 'rtconfig.h', 'stm32f10x.h' }
$files += Get-ChildItem (Join-Path $root 'firmware\gd32f303-main\SYSTEM') -File -Recurse | Where-Object { $_.Extension -in '.c', '.h' }
$files += Get-ChildItem (Join-Path $root 'firmware\gd32f303-main\HAREWARE') -File -Recurse |
    Where-Object { $_.Extension -in '.c', '.h' -and $_.Name -ne 'oledfont.h' -and $_.FullName -notmatch '\\ICM20948\\' }
$files += Get-ChildItem (Join-Path $root 'firmware\gd32f303-main\BALANCE') -File -Recurse | Where-Object { $_.Extension -in '.c', '.h' }
$files += Get-ChildItem (Join-Path $root 'firmware\stm32f103-spray\APP') -File -Recurse | Where-Object { $_.Extension -in '.c', '.h' }
$files += Get-ChildItem (Join-Path $root 'firmware\stm32f103-spray\BOARD') -File -Recurse | Where-Object { $_.Extension -in '.c', '.h' }
$files += Get-ChildItem (Join-Path $root 'firmware\stm32f103-spray\User') -File | Where-Object { $_.Name -in 'main.c', 'rtconfig.h' }
$files += Get-ChildItem (Join-Path $root 'firmware\stm32f103-spray\System') -File -Recurse | Where-Object { $_.Extension -in '.c', '.h' }
$files += Get-ChildItem (Join-Path $root 'firmware\stm32f103-spray\Hardware') -File | Where-Object { $_.Extension -in '.c', '.h' -and $_.Name -ne 'OLED_Font.h' }

$files = $files | Sort-Object FullName -Unique
foreach ($file in $files) {
    & $formatter.Source -i --style=file $file.FullName
    if ($LASTEXITCODE -ne 0) {
        throw "clang-format failed: $($file.FullName)"
    }
}
Write-Host "Formatted $($files.Count) project-owned files."
