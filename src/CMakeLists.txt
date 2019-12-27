project(InCTL)

file(GLOB allocator ./allocator/*)
file(GLOB container ./container/* ./adapter/*)
file(GLOB adapter ./adapter/*)

#add_library(InCTL SHARED ${allocator} ${container})
add_library(InCTL ${allocator} ${container} ${adapter})