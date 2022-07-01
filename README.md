# CTL
C Language Template Library<br>

## Install
```
//use vcpkg install
vcpkg install air-ctl

// or use git
git clone https://github.com/inie0722/CTL
cd CTL
mkdir build && cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
cmake --build ./ --config Release --target install

//run test
cmake ../ -DCMAKE_BUILD_TYPE=Release -DTest=ON
make check && ctest
```

## Container task lists
- [x] `array`
- [x] `vector`
- [x] `deque`
- [x] `list`
- [x] `hash map`
- [x] `tree map`
- [x] `queue`
- [x] `stack`
- [x] `heap`
- [x] `priority queue`

## Algorithm task lists 
- [x] `heap sort`
- [x] `insertion sort`
- [x] `intro sort`
- [x] `quick sort`
- [x] `tim sort`

## Lockfree task lists 
- [x] `spsc queue`
- [x] `mpmc queue`
- [x] `queue`
- [x] `stack`
- [x] `aba pointer`
- [x] `allocator`


## About
This library is a C language container library written by imitating STL, you can use this library like STL<br>
If you want to see the specific container usage method, you can go to the [documentation](https://inie0722.github.io/CTL/)
The code implementation is modeled after SGI-STL<br>

## Note
For memory overflow, function callback is used, which means that all APIs will not return the error of memory allocation failure, but you need to set the OOM callback function yourself<br>
```c
void CTL_set_malloc_handler(void (*handler)()); //Memory allocation failure Handler function Defined 
```

__This library provides two different map__<br>
[tree_map](https://inie0722.github.io/CTL/tree__map_8h.html) Implemented using a red-black tree, allowing the same key to be typed, and using a callback function for key comparison and judgment<br>
[hash_map](https://inie0722.github.io/CTL/hash__map_8h.html) Implemented using an open-chain hash table, it is not allowed to enter the same key, and use memcmp for key comparison judgment, you need to pay extra attention to this<br>

__Sort Algorithm__<br>
This library only provides a [heap sort](https://inie0722.github.io/CTL/heap_8h.html), which is suitable for containers that support random access such as deque vector array<br>
If you need to use other sort algorithms, you can use [algorithm](https://inie0722.github.io/CTL/dir_dbd12e429e2e67fe2605de08ad0007a6.html)<br>
It provides two advanced sort algorithms [intro_sort](https://inie0722.github.io/CTL/intro__sort_8h.html) and [tim_sort](https://inie0722.github.io/CTL/tim__sort_8h.html)<br>
The former is unstable, and it is faster to sort out-of-order data<br>
The latter is stable, and sorting is faster for partially ordered data<br>

## Other
For the API, I can't promise any compatibility<br>
Because of my limited ability, I cannot ensure that every API is correct. If you find a bug, please let me know in time<br>
If you like this library, you can give me a star<br>