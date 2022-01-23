cmake_minimum_required(VERSION 3.15)
project(fastcpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(fastcpp main.cpp)

add_compile_options(-O2)

target_link_libraries(fastcpp pthread)
