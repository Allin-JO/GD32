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
$files += Get-ChildItem (Join-Path $root 'firmware\gd32f303-main\APP') -File -Include *.c,*.h -Recurse
$files += Get-ChildItem (Join-Path $root 'firmware\gd32f303-main\BOARD') -File -Include *.c,*.h -Recurse
$files += Get-ChildItem (Join-Path $root 'firmware\gd32f303-main\USER') -File -Include main.c,rtconfig.h,stm32f10x.h
$files += Get-ChildItem (Join-Path $root 'firmware\gd32f303-main\SYSTEM') -File -Include *.c,*.h -Recurse
$files += Get-ChildItem (Join-Path $root 'firmware\gd32f303-main\HAREWARE') -File -Include *.c,*.h -Recurse |
    Where-Object { $_.Name -ne 'oledfont.h' -and $_.FullName -notmatch '\\ICM20948\\' }
$files += Get-ChildItem (Join-Path $root 'firmware\gd32f303-main\BALANCE') -File -Include *.c,*.h -Recurse
$files += Get-ChildItem (Join-Path $root 'firmware\stm32f103-spray\APP') -File -Include *.c,*.h -Recurse
$files += Get-ChildItem (Join-Path $root 'firmware\stm32f103-spray\BOARD') -File -Include *.c,*.h -Recurse
$files += Get-ChildItem (Join-Path $root 'firmware\stm32f103-spray\User') -File -Include main.c,rtconfig.h
$files += Get-ChildItem (Join-Path $root 'firmware\stm32f103-spray\System') -File -Include *.c,*.h -Recurse
$files += Get-ChildItem (Join-Path $root 'firmware\stm32f103-spray\Hardware') -File -Include *.c,*.h |
    Where-Object { $_.Name -ne 'OLED_Font.h' }

$files = $files | Sort-Object FullName -Unique
foreach ($file in $files) {
    & $formatter.Source -i --style=file $file.FullName
    if ($LASTEXITCODE -ne 0) {
        throw "clang-format failed: $($file.FullName)"
    }
}
Write-Host "Formatted $($files.Count) project-owned files."
