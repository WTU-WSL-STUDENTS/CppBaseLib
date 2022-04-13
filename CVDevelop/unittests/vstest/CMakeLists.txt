# CMakeList.txt: vstest 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("vstest")


set(CMAKE_CXX_STANDARD 17)
add_definitions(-D__WINDOWS)
add_definitions(-DMSVC_64) 
set(LIB_Path ../../../../../build/lib/x64)

include_directories(../../../Common/include)
include_directories(../../include)
include_directories(../../thirdparty/lua-5.4.3/src)


add_library(lua_test SHARED ../LuaExport.cpp)
target_link_libraries(lua_test ${LIB_Path}/lua54)
add_executable(calllua_test ../CallLua.cpp)
target_link_libraries(calllua_test ${LIB_Path}/lua54)

