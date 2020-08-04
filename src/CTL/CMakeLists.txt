project(libc_ctl)

file(GLOB allocator ./allocator/*)
file(GLOB container ./container/* ./adapter/*)
file(GLOB adapter ./adapter/*)
file(GLOB algorithm ./algorithm/*)

add_library(libc_ctl SHARED ${allocator} ${container} ${adapter} ${algorithm})

#是否开启分配器
#add_definitions(-D CTL_NO_ALLOCATE)

target_link_libraries(libc_ctl pthread)