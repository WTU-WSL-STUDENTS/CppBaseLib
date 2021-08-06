copy ..\cmake\CMakeLists.txt ..\build
copy ..\cmake\CommonUnitTests.cmake ..\build
@REM 设置指定CMakeLists.txt用vs2019的编译器进行Win32编译
cmake -G "Visual Studio 16 2019" -A Win32 ..\build\CMakeLists.txt
@REM 设置x64平台进行编译,如果切换平台进行编译，需要将..\build目录内的文件全部删除。 我这台电脑默认也是x64 cmake ../build/CMakeLists.txt
@REM cmake -G "Visual Studio 16 2019" -A x64 ..\build\CMakeLists.txt
cmake --build ..\build 