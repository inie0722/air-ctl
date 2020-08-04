cmake_minimum_required(VERSION 3.00)
project(libc)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

include_directories(include/)
include_directories(include/CTL)
include_directories(include/algorithm)

add_subdirectory(test)

add_subdirectory(src)