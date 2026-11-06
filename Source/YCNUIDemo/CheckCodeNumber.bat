@echo off
setlocal enabledelayedexpansion
chcp 65001 >nul

set cpp_lines=0
set h_lines=0
set total=0

echo 正在统计...

for /r %%f in (*.cpp) do (
    for /f %%c in ('type "%%f" ^| find /c /v ""') do set /a cpp_lines+=%%c
)

for /r %%f in (*.h) do (
    for /f %%c in ('type "%%f" ^| find /c /v ""') do set /a h_lines+=%%c
)

set /a total=cpp_lines+h_lines

echo ================================
echo   .cpp 文件行数 : %cpp_lines%
echo   .h  文件行数  : %h_lines%
echo --------------------------------
echo   总行数        : %total%
echo ================================
pause