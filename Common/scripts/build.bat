echo off
@REM ±àÒëµ¥Ôª²âÊÔ
cd ..\build
cmake ..\ -G "Visual Studio 16 2019" -A win32
cmake --build .
cd ..\scripts 