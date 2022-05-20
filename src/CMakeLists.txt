file(GLOB allocator ./allocator/*)
file(GLOB container ./container/* ./adapter/*)
file(GLOB adapter ./adapter/*)
file(GLOB algorithm ./algorithm/*)

add_library(CTL_shared SHARED ${allocator} ${container} ${adapter} ${algorithm})
target_link_libraries(CTL_shared pthread)

add_library(CTL_static STATIC ${allocator} ${container} ${adapter} ${algorithm})

set_target_properties(CTL_shared PROPERTIES OUTPUT_NAME "CTL")
set_target_properties(CTL_static PROPERTIES OUTPUT_NAME "CTL")