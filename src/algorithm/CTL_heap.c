#include <malloc.h>
#include <alloca.h>
#include <string.h>

#include "CTL_heap.h"

static void __push_heap(CTL_functions *functions, CTL_iterator *first, ptrdiff_t holeIndex, ptrdiff_t topIndex, void *value, bool (*compare_handler)(void *a, void *b));
static void __adjust_heap(CTL_functions *functions, CTL_iterator *first, ptrdiff_t holeIndex, ptrdiff_t len, void *value, bool (*compare_handler)(void *a, void *b));

void CTL_push_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(void *a, void *b))
{
    void *value = alloca(first->T_size);
    CTL_iterator back;
    functions->iterator_move(last, -1, &back);
    memcpy(value, back.data, first->T_size);

    __push_heap(functions, first, functions->iterator_diff(last, first) - 1, 0, value, compare_handler);
}

static void __push_heap(CTL_functions *functions, CTL_iterator *first, ptrdiff_t holeIndex, ptrdiff_t topIndex, void *value, bool (*compare_handler)(void *a, void *b))
{
    CTL_iterator move_result;

    // __push_heap 从holeIndex开始执行上溯操作 子节点和父节点比较 如果子节点 大于 父节点 便进行交换 子节点变成父节点
    //holeIndex 插入点位置 topIndex根节点位置
    ptrdiff_t parent = (holeIndex - 1) / 2; //holeIndex的父节点位置

    //holeIndex的父节点 如果小于holeIndex 则和holeIndex位置进行交换
    while (holeIndex > topIndex && compare_handler((functions->iterator_move(first, parent, &move_result), move_result.data), value))
    {
        //第一个holeIndex的值已经被保存到value里了 所以这里直接赋值
        memcpy((functions->iterator_move(first, holeIndex, &move_result), move_result.data), (functions->iterator_move(first, parent, &move_result), move_result.data), first->T_size);

        //这里和上面一样 holeIndex变成parent
        holeIndex = parent;
        //parent变成parent的父节点
        parent = (holeIndex - 1) / 2;
    }

    //将数据插入到合适位置
    memcpy((functions->iterator_move(first, holeIndex, &move_result), move_result.data), value, first->T_size);
}

void CTL_pop_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(void *a, void *b))
{
    //保存数据
    void *value = alloca(first->T_size);

    CTL_iterator back;
    functions->iterator_move(last, -1, &back);
    memcpy(value, back.data, first->T_size);

    memcpy(back.data, first->data, first->T_size);
    __adjust_heap(functions, first, 0, functions->iterator_diff(last, first) - 1, value, compare_handler);
}

static void __adjust_heap(CTL_functions *functions, CTL_iterator *first, ptrdiff_t holeIndex, ptrdiff_t len, void *value, bool (*compare_handler)(void *a, void *b))
{
    CTL_iterator move_result;

    ptrdiff_t topIndex = holeIndex;

    ptrdiff_t secondChild = 2 * holeIndex + 2;

    while (secondChild < len)
    {
        //比较左右节点 将较大的子节点替换到父节点位置
        if (compare_handler((functions->iterator_move(first, secondChild, &move_result), move_result.data), (functions->iterator_move(first, secondChild - 1, &move_result), move_result.data)))
            --secondChild;

        memcpy((functions->iterator_move(first, holeIndex, &move_result), move_result.data), (functions->iterator_move(first, secondChild, &move_result), move_result.data), first->T_size);
        holeIndex = secondChild;
        //下一个字节点位置
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len)
    {
        //右节点为空 将左节点移动到父节点位置
        memcpy((functions->iterator_move(first, holeIndex, &move_result), move_result.data), (functions->iterator_move(first, secondChild - 1, &move_result), move_result.data), first->T_size);
        holeIndex = secondChild - 1;
    }
    //重新调整堆 在合适的位置插入 value 及是 先前last-1的值
    //不能直接在holeIndex节点插入 因为holeIndex的父节点 可能小于value
    __push_heap(functions, first, holeIndex, topIndex, value, compare_handler);
}

void CTL_sort_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(void *a, void *b))
{
    CTL_iterator w_last = *last;
    //通过循环出堆 进行排序 大堆 变成 从小到大排序
    while (functions->iterator_diff(&w_last, first) > 1)
    {
        CTL_pop_heap(functions, first, &w_last, compare_handler);
        functions->iterator_move(&w_last, -1, &w_last);
    }
}

void CTL_make_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(void *a, void *b))
{
    ptrdiff_t len = functions->iterator_diff(last, first);
    //数据量 小于或等于2 直接返回 无需操作
    if (len < 2)
        return;

    //parent是第一个需重新排列的子树头部
    ptrdiff_t parent = (len - 2) / 2;
    void *value = alloca(first->T_size);

    while (parent >= 0)
    {
        CTL_iterator move_result;
        //len 是防止越界的 如果子节点 大于 父节点 便进行交换
        memcpy(value, (functions->iterator_move(first, parent, &move_result), move_result.data), first->T_size);
        __adjust_heap(functions, first, parent, len, value, compare_handler);
        parent--;
    }
}
