file(GLOB allocator ./allocator/*)
file(GLOB container ./container/* ./adapter/*)
file(GLOB adapter ./adapter/*)
file(GLOB algorithm ./algorithm/*)

if(BUILD_SHARED_LIBS)
  add_library(CTL ${allocator} ${container} ${adapter} ${algorithm})
else()
  add_library(CTL STATIC ${allocator} ${container} ${adapter} ${algorithm})
endif()

set_target_properties(CTL PROPERTIES OUTPUT_NAME "ctl")

if(UNIX OR APPLE)
    target_link_libraries(CTL pthread)
endif()