echo off
@REM ±‡“Îµ•‘™≤‚ ‘
cd ..\build
cmake ..\ -G "Visual Studio 16 2019" -A win32 
cmake --build . --config Release
cd ..\scripts 