project(InCTL)

file(GLOB allocator ./allocator/*  ./allocator/*)
file(GLOB container ./container/*)

#add_library(InCTL SHARED ${allocator} ${container})
add_library(InCTL ${allocator} ${container})