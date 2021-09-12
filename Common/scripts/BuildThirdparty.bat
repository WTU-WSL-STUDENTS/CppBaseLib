@REM ±àÒëÈı·½ÒÀÀµ
copy ..\thirdparty\CMakeLists.txt ..\build
copy ..\thirdparty\thirdparty.cmake ..\build
cmake -G "Visual Studio 16 2019" -A Win32 ..\build\CMakeLists.txt
cmake --build ..\build 