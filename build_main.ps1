$ErrorActionPreference = "Stop"

$vcvars = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat"
$qmake = "C:\Qt\Qt5.12.11\5.12.11\msvc2017_64\bin\qmake.exe"
$proPath = "D:\9706_draintank_EN\MainFrame\HIFUMainFrameWithOperatorNavigation\HIFUMainframe.pro"
$proDir = "D:\9706_draintank_EN\MainFrame\HIFUMainFrameWithOperatorNavigation"

Write-Host "=== Rebuilding HIFUMainFrame ===" -ForegroundColor Cyan

$script = @"
call "$vcvars" x64
cd /d "$proDir"
"$qmake" "$proPath" -spec win32-msvc CONFIG+=release CONFIG-=debug CONFIG-=debug_and_release CONFIG+=force_debug_info
nmake /f Makefile.Release clean 2>nul
nmake /f Makefile.Release
"@

$result = cmd /c $script 2>&1
$result | Out-Host

$exe = "D:\9706_draintank_EN\MainFrame\build-HIFUMainframe-Desktop_Qt_5_12_11_MSVC2017_64bit-Release\release\HIFUMainFrame.exe"
if (Test-Path $exe) {
    $size = (Get-Item $exe).Length
    Write-Host "HIFUMainFrame.exe built: $size bytes" -ForegroundColor Green
    # Check for Chinese strings
    $tmpFile = [System.IO.Path]::GetTempFileName()
    [System.IO.File]::WriteAllBytes($tmpFile, [System.IO.File]::ReadAllBytes($exe))
    $content = [System.Text.Encoding]::UTF8.GetString([System.IO.File]::ReadAllBytes($exe))
    $chineseMatches = [System.Text.RegularExpressions.Regex]::Matches($content, "[\u4e00-\u9fff]+")
    if ($chineseMatches.Count -gt 0) {
        $unique = $chineseMatches.Value | Sort-Object -Unique
        Write-Host "Found $($unique.Count) unique Chinese strings in exe:" -ForegroundColor Yellow
        $unique | ForEach-Object { Write-Host "  $_" }
    } else {
        Write-Host "NO Chinese strings found in exe!" -ForegroundColor Green
    }
    Remove-Item $tmpFile -Force
} else {
    Write-Host "ERROR: Exe not built!" -ForegroundColor Red
}

Write-Host "=== Done ===" -ForegroundColor Cyan
