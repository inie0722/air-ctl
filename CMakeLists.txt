cmake_minimum_required(VERSION 3.00)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)


#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

#add_definitions(-D CTL_NO_ALLOCATE)

add_definitions(-Wall -O3)

include_directories(include/)

add_subdirectory(test)

add_subdirectory(src)