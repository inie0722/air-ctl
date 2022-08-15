file(GLOB container container/*)
file(GLOB adapter adapter/*)
file(GLOB algorithm algorithm/*)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(lockfree lockfree/spsc_queue.c lockfree/mpmc_queue.c lockfree/queue.c lockfree/stack.c lockfree/allocator.c)
else()
	set(lockfree lockfree/spsc_queue.c lockfree/mpmc_queue.c)
endif()

if(BUILD_SHARED_LIBS)
	add_compile_definitions(CTL_DLL_BUILD)
  	add_library(${PROJECT_NAME} SHARED ${allocator} ${container} ${adapter} ${algorithm} ${lockfree} malloc.c)
else()
	add_library(${PROJECT_NAME} STATIC ${allocator} ${container} ${adapter} ${algorithm} ${lockfree} malloc.c)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "airctl")

if(UNIX OR APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()