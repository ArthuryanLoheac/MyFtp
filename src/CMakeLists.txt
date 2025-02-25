#Set the minimum version of CMake that can be used
cmake_minimum_required(VERSION 3.17.0)

#Set the project name to hello_world
project(myftp)

#Set the C++ standard to use
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE NANO_SRC "./**.cpp")
include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB_RECURSE HEADER_FILES "../include/**.hpp")

foreach(HEADER_FILE ${HEADER_FILES})
    get_filename_component(HEADER_DIR ${HEADER_FILE} DIRECTORY)
    include_directories(${HEADER_DIR})
endforeach()

#create exectuable hello from hello_world.c
add_executable(${PROJECT_NAME} ${NANO_SRC})