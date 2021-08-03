copy ..\cmake\CMakeLists.txt ..\build
copy ..\cmake\CommonUnitTests.cmake ..\build
cmake ../build/CMakeLists.txt
cmake --build ../build