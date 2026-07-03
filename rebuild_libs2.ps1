$ErrorActionPreference = "Stop"

$vcvars = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat"
$qmake = "C:\Qt\Qt5.12.11\5.12.11\msvc2017_64\bin\qmake.exe"

$libs = @(
    @{ Name="HIFUConnection"; Pro="D:\9706_draintank_EN\HIFUConnection\HIFUConnection\HIFUConnection.pro" },
    @{ Name="HIFUNavigation"; Pro="D:\9706_draintank_EN\HIFUNavigation\HIFUNavigation\HIFUNavigation.pro" }
)

foreach ($lib in $libs) {
    $name = $lib.Name
    $proPath = $lib.Pro
    $proDir = Split-Path $proPath -Parent
    Write-Host "=== Building $name ===" -ForegroundColor Cyan

    $script = @"
call "$vcvars" x64
cd /d "$proDir"
"$qmake" "$proPath" CONFIG+=release CONFIG-=debug CONFIG-=debug_and_release
nmake /f Makefile.Release clean 2>nul
nmake /f Makefile.Release
"@

    $result = cmd /c $script 2>&1
    # Filter out the verbose compiler output, only show errors
    $result | Where-Object { $_ -match 'error|Error|ERROR|warning|Warning|WARNING|===|rebuilt|Build' } | Out-Host

    $libFile = "D:\9706_draintank_EN\bin\$name.lib"
    if (Test-Path $libFile) {
        $size = (Get-Item $libFile).Length
        Write-Host "$name.lib rebuilt: $size bytes" -ForegroundColor Green
    } else {
        Write-Host "WARNING: $name.lib not found at bin!" -ForegroundColor Red
    }
    Write-Host ""
}

Write-Host "=== Done ===" -ForegroundColor Cyan
