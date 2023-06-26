cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
project(merger) #name of your project

add_executable(merger main.cpp)

target_compile_options(merger PRIVATE -Wall -ggdb3)
target_link_libraries(merger rt stdc++fs)

