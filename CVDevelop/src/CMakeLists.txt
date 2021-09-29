include_directories(../thirdparty/lua-5.4.3/src)
add_library(lua_test SHARED luaExportTest.cpp)
target_link_libraries(lua_test ../${LIB_Path}/lua54)