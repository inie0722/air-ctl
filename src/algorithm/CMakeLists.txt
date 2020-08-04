project(libc_algorithm)


file(GLOB f ./*)

add_library(libc_algorithm SHARED ${f})