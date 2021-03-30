cmake_minimum_required(VERSION 3.10)
project(tinyplayer)

include(FindPkgConfig)
include(CTest)

set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
#set(SOURCES src/main.cpp)
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

#DOCS
find_package(Doxygen)
if(Doxygen_FOUND)
  add_subdirectory(docs)
else()
  message(STATUS "Doxygen not found, not building docs")
endif()

#SOURCE CODE
add_subdirectory(src)

#EXECUTABLES
add_subdirectory(app)

#TESTING
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
