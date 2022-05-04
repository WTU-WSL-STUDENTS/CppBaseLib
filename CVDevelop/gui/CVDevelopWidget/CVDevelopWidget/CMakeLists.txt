cmake_minimum_required(VERSION 3.5)

project(CVDevelopWidget LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH $ENV{QTDIR}/msvc2017_64)

set(Qt5_DIR             ${CMAKE_PREFIX_PATH}/lib/cmake/Qt5)
set(Qt5Widgets_DIR      ${CMAKE_PREFIX_PATH}/lib/cmake/Qt5Widgets)
set(Qt5Gui_DIR          ${CMAKE_PREFIX_PATH}/lib/cmake/Gui)
set(Qt5Core_DIR         ${CMAKE_PREFIX_PATH}/lib/cmake/Qt5Core)
set(QT_QMAKE_EXECUTABLE ${CMAKE_PREFIX_PATH}/bin/qmake.exe)

find_package(Qt5 COMPONENTS Widgets REQUIRED)


add_executable(CVDevelopWidget
  main.cpp
  CVDevelopWidget.cpp
  CVDevelopWidget.h
  CVDevelopWidget.ui
)

target_link_libraries(CVDevelopWidget PRIVATE Qt5::Widgets)
