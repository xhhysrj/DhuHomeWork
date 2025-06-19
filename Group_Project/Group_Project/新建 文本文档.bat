@echo off
chcp 65001 > nul  & REM 设置命令行编码为 UTF-8，避免中文路径乱码
echo 开始转换当前目录下的 .txt、.cpp、.h 文件为 Unicode 编码（UTF-16LE）...

REM 调用 PowerShell 执行编码转换
powershell -Command "Get-ChildItem -Path . -Include *.txt,*.cpp,*.h -File | ForEach-Object {
    $content = Get-Content -Path $_.FullName -Encoding Byte
    Set-Content -Path $_.FullName -Value $content -Encoding Unicode
}"

echo 转换完成！
pause