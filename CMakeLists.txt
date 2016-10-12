cmake_minimum_required(VERSION 2.8.4)
project(csci3150_asg2_release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    asg2-shell.c)

add_executable(csci3150_asg2_release ${SOURCE_FILES})
