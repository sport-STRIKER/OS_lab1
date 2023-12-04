cmake_minimum_required(VERSION 3.10)

project(OS_Lab1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main Parent.cpp)
add_executable(Child1 Child1.cpp)
add_executable(Child2 Child2.cpp)

