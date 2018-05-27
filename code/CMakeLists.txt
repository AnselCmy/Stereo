cmake_minimum_required(VERSION 3.7)
project(code)
find_package(OpenCV3)
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)

add_executable(code main.cpp StereoRectification.h)
find_package(OpenCV REQUIRED)
target_link_libraries(code ${OpenCV_LIBS})