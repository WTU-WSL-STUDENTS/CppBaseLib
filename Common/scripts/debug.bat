echo off
if not exist ..\build md ..\build
cd ..\build
cmake -DBUILD_3RDPART=OFF ..\ -G "Visual Studio 16 2019" -A win32 
cmake --build .