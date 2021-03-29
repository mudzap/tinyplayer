cmake_minimum_required(VERSION 3.10)
project(tinyplayer)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
#set(SOURCES src/main.cpp)
file(GLOB SOURCES "src/*.cpp")

add_executable(tinyplayer ${SOURCES})

