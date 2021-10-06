# tinyxml
include_directories(tinyxml-2-6-2)
file(GLOB xmllib_src tinyxml-2-6-2/*.cpp)
add_library(xmllib ${xmllib_src})

# ftplib
include_directories(ftplib-4.0-1)
file(GLOB ftplib_src ftplib-4.0-1/*.c)
add_library(ftplib ${ftplib_src})

# sqlite 
include_directories(sqlite-3-36)
file(GLOB sqlite_src sqlite-3-36/*.c)
add_executable(sqlite3exe ${sqlite_src}) 
add_library(sqlite3lib sqlite-3-36/sqlite3.c)
add_library(sqlite3dll SHARED sqlite-3-36/sqlite3.c)
set_target_properties(sqlite3exe PROPERTIES OUTPUT_NAME  sqlite3)
set_target_properties(sqlite3lib PROPERTIES OUTPUT_NAME  sqlite3)
set_target_properties(sqlite3dll PROPERTIES OUTPUT_NAME  sqlite3)

# glad download from  https://glad.dav1d.de/
include_directories(glad/include)
file(GLOB glad_src glad/src/*.c) 
add_library(glad ${glad_src})

# 重定向glfw生成路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	../../../${RELEASE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	../../../${RELEASE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	../../../${LIB_Path})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE 	../../../${LIB_Path})
add_subdirectory(glfw-3.3.4)
# 复原
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	../${RELEASE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	../${RELEASE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	../${LIB_Path})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE 	../${LIB_Path})