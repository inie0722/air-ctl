# InCTL
C Language Template Library C语言 模板库<br>
__安装__
```
git clone https://github.com/inie0722/libc
cd libc
mkdir build && cd build
cmake ../
//生成全部文件
cmake --build ./
//如果你不需要 生成 单元测试 可以使用命令
cmake --build ./ --config --target libc_ctl
```

## 关于
本库是仿照STL书写的C语言容器库, 你可以像STL那样使用本库<br>
通过CMake构造本库,采用gtest框架书写的单元测试,所有单元测试都放置到test文件夹中<br>
如果你想查看具体容器使用例子,请自行参照test文件夹中的单元测试<br>
代码实现是仿照SGI-STL书写的<br>

## 注意
本库提供了一个分配器,你可以通过预编译宏CTL_NO_ALLOCATE禁用改分配器<br>
对于内存溢出采用函数回调处理,这意味着所有API均不会返回内存分配失败的错误,但是你需要自行设置OOM回调函数<br>
```c
void CTL_set_malloc_handler(void (*handler)()); //内存分配失败 处理函数 由用户自定义
```
通过该函数设置OOM回调函数<br>

__本库提供了两种不同的map__<br>
tree_map 使用红黑树实现, 允许键入相同的Key, 使用回调函数进行Key比较判断<br>
hash_map 使用开链式hash表实现, 不允许键入相同Key, 使用memcmp进行Key比较判断, 你需要额外注意这点<br>

__排序算法__<br>
本库仅提供了一个堆排序,适用于deque vector array等支持随机访问的容器<br>
如果你需要使用其他排序算法,可以去使用[InSort](https://github.com/inie0722/InSort)<br>
[InSort](https://github.com/inie0722/InSort)可以对数组进行排序,就像qsort一样<br>
它提供了两种高级排序算法 intro_sort 和 tim_sort<br>
前者不稳定,对于乱序数据排序比较快<br>
后者稳定,对于部分有序数据排序比较快<br>

## 其他
对于API,我无法承诺任何的兼容性<br>
因为本人能力有限,无法确保每一个API都是正确无误的,如果你发现了bug可以及时告诉我<br>
如果你喜欢本库,可以给予我一个star<br>