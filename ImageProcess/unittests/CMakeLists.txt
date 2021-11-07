include_directories(../include)
include_directories(../../Common/include)
add_executable(ImageMat.Test ImageMat.Test.cpp)
target_link_libraries(ImageMat.Test gdiplus)

add_executable(GdiImageIO.Test GdiImageIO.Test.cpp)
target_link_libraries(GdiImageIO.Test gdiplus)

add_executable(BmpImageIO.Test BmpImageIO.Test.cpp)

add_executable(ImageTransformation.Test ImageTransformation.Test.cpp)

add_executable(Rle.Test Rle.Test.cpp)