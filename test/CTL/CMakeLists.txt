add_executable(array array.cpp)

add_executable(vector vector.cpp)

add_executable(list list.cpp)

add_executable(deque deque.cpp)

add_executable(queue queue.cpp)

add_executable(stack stack.cpp)

add_executable(heap heap.cpp)

add_executable(priority_queue priority_queue.cpp)

add_executable(tree_map tree_map.cpp)

add_executable(hash_map hash_map.cpp)

target_link_libraries(array gtest pthread CTL_shared)

target_link_libraries(vector gtest pthread CTL_shared)

target_link_libraries(list gtest pthread CTL_shared)

target_link_libraries(deque gtest pthread CTL_shared)

target_link_libraries(queue gtest pthread CTL_shared)

target_link_libraries(stack gtest pthread CTL_shared)

target_link_libraries(heap gtest pthread CTL_shared)

target_link_libraries(priority_queue gtest pthread CTL_shared)

target_link_libraries(tree_map gtest pthread CTL_shared)

target_link_libraries(hash_map gtest pthread CTL_shared)