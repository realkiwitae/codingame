cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(spring-challenge-2023) #name of your project

add_executable(myapp main.cpp CubeCoord.cpp Cell.cpp Action.cpp Player.cpp Board.cpp ./utils/draw.cpp)

find_package( OpenCV 4.5.0 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_compile_options(myapp PRIVATE -Wall -ggdb3)
target_link_libraries(myapp rt ${OpenCV_LIBS} pthread)
