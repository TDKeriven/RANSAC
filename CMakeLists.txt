cmake_minimum_required(VERSION 3.3)
project(RANSAC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
include_directories(include/)
add_executable(RANSAC ${SOURCE_FILES})