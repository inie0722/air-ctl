project(test)

add_executable(array array.cpp)

add_executable(vector vector.cpp)

add_executable(list list.cpp)

add_executable(deque deque.cpp)

target_link_libraries(array gtest pthread InCTL)

target_link_libraries(vector gtest pthread InCTL)

target_link_libraries(list gtest pthread InCTL)

target_link_libraries(deque gtest pthread InCTL)