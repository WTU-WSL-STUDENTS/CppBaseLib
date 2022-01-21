echo off
if not exist ..\build md ..\build
cd ..\build
cmake ..\ -G "Visual Studio 16 2019" -A win32 
cmake --build . --config Release
copy ..\thirdparty\fftw-3.3.5\x86\*.dll .\bin\x86\
cd ..\scripts 