cmake_minimum_required(VERSION 3.00)
project(CTL)

if(WIN32)
    option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()


set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

include_directories(${CMAKE_SOURCE_DIR}/include/)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)

if(Test)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    add_subdirectory(${CMAKE_SOURCE_DIR}/test)
endif()

install(TARGETS CTL
        RUNTIME DESTINATION bin
    	LIBRARY DESTINATION lib
    	ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/CTL
    	DESTINATION include
        FILES_MATCHING
        PATTERN *.h
)