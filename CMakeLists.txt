cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(OpenCV_STATIC OFF)
find_package( OpenCV REQUIRED )
include_directories(include/ ${OpenCV_INCLUDE_DIRS})

project(RANSAC)
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

add_executable(RANSAC ${SOURCE_FILES} src/test.cpp include/test.h src/linearmodel.cpp include/linearmodel.h src/utils.cpp include/utils.h include/homography.h src/homography.cpp)
target_link_libraries(RANSAC ${OpenCV_LIBS} )