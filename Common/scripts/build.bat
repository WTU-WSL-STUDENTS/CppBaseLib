echo off
@REM ���뵥Ԫ����
cd ..\build
cmake ..\ -G "Visual Studio 16 2019" -A win32
cmake --build .
cd ..\scripts 