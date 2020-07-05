file(GLOB allocator ./allocator/*)
file(GLOB container ./container/* ./adapter/*)
file(GLOB adapter ./adapter/*)
file(GLOB algorithm ./algorithm/*)

add_library(InCTL SHARED ${allocator} ${container} ${adapter} ${algorithm})

#add_definitions(-D CTL_NO_ALLOCATE)

add_definitions(-Wall -O3 -std=c11)
target_link_libraries(InCTL pthread)