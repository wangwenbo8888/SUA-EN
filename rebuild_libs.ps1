$ErrorActionPreference = "Stop"

# Set up MSVC 2017 64-bit environment
$vcvars = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat"
$qmake = "C:\Qt\Qt5.12.11\5.12.11\msvc2017_64\bin\qmake.exe"

# Libraries to rebuild (source .pro files)
$libs = @(
    @{ Name="PlanScene3D"; Pro="D:\9706_draintank_EN\HIFUPlan\PlanScene3D\PlanScene3D.pro" },
    @{ Name="NewScene3D"; Pro="D:\9706_draintank_EN\HIFUPlan\NewScene3D\NewScene3D.pro" },
    @{ Name="Client"; Pro="D:\9706_draintank_EN\HIFUNetwork\Client\Client.pro" }
)

foreach ($lib in $libs) {
    $name = $lib.Name
    $proPath = $lib.Pro
    $proDir = Split-Path $proPath -Parent
    Write-Host "=== Building $name ===" -ForegroundColor Cyan

    # Run qmake + nmake in a cmd.exe that first calls vcvarsall
    $script = @"
call "$vcvars" x64
cd /d "$proDir"
"$qmake" "$proPath" -spec win32-msvc CONFIG+=release CONFIG-=debug CONFIG-=debug_and_release
nmake /f Makefile.Release clean 2>nul
nmake /f Makefile.Release
"@

    $result = cmd /c $script 2>&1
    $result | Out-Host

    # Check if .lib was produced
    $libFile = "D:\9706_draintank_EN\bin\$name.lib"
    if (Test-Path $libFile) {
        $size = (Get-Item $libFile).Length
        Write-Host "$name.lib rebuilt: $size bytes" -ForegroundColor Green
    } else {
        $libFile = Join-Path $proDir "release\$name.lib"
        if (Test-Path $libFile) {
            $size = (Get-Item $libFile).Length
            Write-Host "$name.lib rebuilt (in source dir): $size bytes" -ForegroundColor Green
        } else {
            Write-Host "WARNING: $name.lib not found!" -ForegroundColor Red
        }
    }
    Write-Host ""
}

Write-Host "=== All libraries built ===" -ForegroundColor Cyan
