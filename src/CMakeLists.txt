project(InCTL)

file(GLOB allocator ./allocator/*)
file(GLOB container ./container/* ./adapter/*)
file(GLOB adapter ./adapter/*)

#add_library(InCTL SHARED ${allocator} ${container})

#target_link_libraries(InCTL pthread)
add_library(InCTL SHARED ${allocator} ${container} ${adapter})