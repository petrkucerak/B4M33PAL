cmake_minimum_required(VERSION 3.5)
project(CascadingSpannigTree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++14 -pipe -Wall -O3")

add_executable(Main main.cpp)