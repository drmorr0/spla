
cmake_minimum_required (VERSION 2.8)
project (SPLA)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS "-Wempty-body -Wall -Wextra ${CMAKE_CXX_FLAGS}")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS_PROFILE "-g -O3 -DNDEBUG")

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)





