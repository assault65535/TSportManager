cmake_minimum_required(VERSION 3.5)
project(TSportManager)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_C_FLAGS "-std=c99")

add_executable(TSportManager Tnecesoc poi.c Tnecesoc/Utils/ArrayMatrix.h Tnecesoc/Authentication.h)