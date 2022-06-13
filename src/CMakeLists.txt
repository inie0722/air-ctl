file(GLOB allocator allocator/*)
file(GLOB container container/*)
file(GLOB adapter adapter/*)
file(GLOB algorithm algorithm/*)
file(GLOB lockfree lockfree/*)

if(BUILD_SHARED_LIBS)
	add_compile_definitions(CTL_DLL_BUILD)
  	add_library(CTL SHARED ${allocator} ${container} ${adapter} ${algorithm} ${lockfree})
else()
	add_library(CTL STATIC ${allocator} ${container} ${adapter} ${algorithm} ${lockfree})
endif()

set_target_properties(CTL PROPERTIES OUTPUT_NAME "ctl")

if(UNIX OR APPLE)
    target_link_libraries(CTL pthread)
endif()