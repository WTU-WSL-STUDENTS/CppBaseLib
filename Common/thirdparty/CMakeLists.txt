include_directories(tinyxml-2-6-2)
file(GLOB xmllib_src tinyxml-2-6-2/*.cpp)
add_library(xmllib ${xmllib_src})

include_directories(ftplib-4.0-1)
file(GLOB ftplib_src ftplib-4.0-1/*.c)
add_library(ftplib ${ftplib_src})

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