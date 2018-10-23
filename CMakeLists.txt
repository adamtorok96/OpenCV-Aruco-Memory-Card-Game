cmake_minimum_required(VERSION 3.10)
project(OpenCVMemoryCardGame)

set(CMAKE_CXX_STANDARD 11)

include(/usr/local/lib/cmake/opencv4/OpenCVConfig.cmake)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(OpenCVMemoryCardGame main.cpp CardGame.cpp CardGame.h)

target_link_libraries(OpenCVMemoryCardGame ${OpenCV_LIBS})