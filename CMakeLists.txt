cmake_minimum_required(VERSION 3.00)
project(CTL)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

include_directories(include/)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME array COMMAND array)
add_test(NAME deque COMMAND deque)
add_test(NAME hash_map COMMAND hash_map)
add_test(NAME heap COMMAND heap)
add_test(NAME list COMMAND list)
add_test(NAME priority_queue COMMAND priority_queue)
add_test(NAME queue COMMAND queue)
add_test(NAME stack COMMAND stack)
add_test(NAME tree_map COMMAND tree_map)
add_test(NAME sort COMMAND sort)