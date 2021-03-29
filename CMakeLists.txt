cmake_minimum_required(VERSION 3.10)
project(tinyplayer)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
#set(SOURCES src/main.cpp)
file(GLOB SOURCES "src/*.cpp")

add_executable(tinyplayer ${SOURCES})

find_package(ALSA REQUIRED)
if (ALSA_FOUND)
    include_directories(${ALSA_INCLUDE_DIRS})
    target_link_libraries (tinyplayer ${ALSA_LIBRARIES})
endif(ALSA_FOUND)

