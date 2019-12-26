cmake_minimum_required(VERSION 3.00)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

#add_definitions(-D CTL_NO_ALLOCATE)

add_definitions(-Wall)

include_directories(include/)

add_subdirectory(test)

add_subdirectory(src)