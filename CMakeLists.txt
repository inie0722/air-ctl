cmake_minimum_required(VERSION 3.00)
project(CTL)
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

TARGET_INCLUDE_DIRECTORIES(CTL INTERFACE $<INSTALL_INTERFACE:include>)

install(TARGETS CTL EXPORT air-ctl-config
        RUNTIME DESTINATION bin
    	LIBRARY DESTINATION lib
    	ARCHIVE DESTINATION lib)
      
install(EXPORT air-ctl-config
        FILE air-ctl-config.cmake
        NAMESPACE air-ctl::
        DESTINATION share/air-ctl
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/CTL
    	DESTINATION include
        FILES_MATCHING
        PATTERN *.h
)