cmake_minimum_required(VERSION 3.5)
project(Exam3)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++14 -pipe -Wall -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++14 -pipe -Wall -O3 -fsanitize=address,undefined")

add_executable(Main main.cpp)