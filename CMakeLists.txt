cmake_minimum_required(VERSION 3.10)
project(tinyplayer)

include(FindPkgConfig)

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

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER_APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GSTREAMER_AUDIO REQUIRED gstreamer-audio-1.0)
pkg_check_modules(GSTREAMER_BASE REQUIRED gstreamer-base-1.0)
pkg_check_modules(GSTREAMER_TAG REQUIRED gstreamer-tag-1.0)
pkg_check_modules(GSTREAMER_PBUTILS REQUIRED gstreamer-pbutils-1.0)

link_directories(${GSTREAMER_LIBRARY_DIRS})

include_directories(${GSTREAMER_INCLUDE_DIRS})
include_directories(${GSTREAMER_APP_INCLUDE_DIRS})
include_directories(${GSTREAMER_AUDIO_INCLUDE_DIRS})
include_directories(${GSTREAMER_BASE_INCLUDE_DIRS})
include_directories(${GSTREAMER_TAG_INCLUDE_DIRS})
include_directories(${GSTREAMER_PBUTILS_INCLUDE_DIRS})

target_link_libraries (tinyplayer
    ${GSTREAMER_BASE_LIBRARIES}
    ${GSTREAMER_LIBRARIES}
    ${GSTREAMER_APP_LIBRARIES}
    ${GSTREAMER_TAG_LIBRARIES}
    ${GSTREAMER_PBUTILS_LIBRARIES}
    )
