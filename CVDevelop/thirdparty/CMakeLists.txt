
include_directories(src)
set(lualib_c lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c ltm.c lundump.c lvm.c lzio.c lauxlib.c lbaselib.c lcorolib.c ldblib.c liolib.c lmathlib.c loadlib.c loslib.c lstrlib.c ltablib.c lutf8lib.c linit.c )
FOREACH(lualib_c ${lualib_c})
  set(lualib_src ${lualib_src} lua-5.4.3/src/${lualib_c})
ENDFOREACH(lualib_c)
add_library(lua54   ${lualib_src})
add_executable(lua  lua-5.4.3/src/lua.c)
target_link_libraries(lua ../${LIB_Path}/lua54)
add_executable(luac  lua-5.4.3/src/luac.c)
target_link_libraries(luac ../${LIB_Path}/lua54)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	../../../${RELEASE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	../../../${RELEASE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	../../../${LIB_Path})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE 	../../../${LIB_Path})
add_subdirectory(ZeroBraneStudio/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	../${RELEASE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	../${RELEASE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	../${LIB_Path})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE 	../${LIB_Path})

