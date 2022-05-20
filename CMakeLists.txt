cmake_minimum_required(VERSION 3.00)
project(CTL)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
enable_testing()

include_directories(include/)

add_subdirectory(src)
add_subdirectory(test)

install(TARGETS CTL_shared CTL_static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)