#include "../vector/CTL_vector.h"

void __push_heap(CTL_vector_iterator *first, ptrdiff_t holeIndex, ptrdiff_t topIndex, type value);
void __adjust_heap(CTL_vector_iterator *first, ptrdiff_t holeIndex, ptrdiff_t len, type value);

void CTL_push_heap(CTL_vector_iterator *first, CTL_vector_iterator *last)
{
    CTL_vector_iterator value = CTL_vector_iterator_move(last, -1);
    __push_heap(first, CTL_vector_iterator_diff(last, first) - 1, 0, *value.data);
}

void __push_heap(CTL_vector_iterator *first, ptrdiff_t holeIndex, ptrdiff_t topIndex, type value)
{
    // __push_heap 从holeIndex开始执行上溯操作 子节点和父节点比较 如果子节点 大于 父节点 便进行交换 子节点变成父节点
    //holeIndex 插入点位置 topIndex根节点位置

    ptrdiff_t parent = (holeIndex - 1) / 2; //holeIndex的父节点位置

    //holeIndex的父节点 如果小于holeIndex 则和holeIndex位置进行交换
    while (holeIndex > topIndex && *CTL_vector_iterator_move(first, parent).data < value)
    {
        //第一个holeIndex的值已经被保存到value里了 所以这里直接赋值
        *CTL_vector_iterator_move(first, holeIndex).data = *CTL_vector_iterator_move(first, parent).data;
        
        //这里和上面一样 holeIndex变成parent
        holeIndex = parent;
        //parent变成parent的父节点
        parent = (holeIndex - 1) / 2;
    }

    //将数据插入到合适位置
    *CTL_vector_iterator_move(first, holeIndex).data = value;
}

void CTL_pop_heap(CTL_vector_iterator *first, CTL_vector_iterator *last)
{
    //保存数据
    type value = *CTL_vector_iterator_move(last, -1).data;
    *CTL_vector_iterator_move(last, -1).data = *first->data;
    __adjust_heap(first, 0, CTL_vector_iterator_diff(last, first) - 1, value);
}

void __adjust_heap(CTL_vector_iterator *first, ptrdiff_t holeIndex, ptrdiff_t len, type value)
{
    ptrdiff_t topIndex = holeIndex;

    ptrdiff_t secondChild = 2 * holeIndex + 2;

    while (secondChild < len)
    {
        //比较左右节点 将较大的子节点替换到父节点位置
        if (*CTL_vector_iterator_move(first, secondChild).data < *CTL_vector_iterator_move(first, secondChild - 1).data)
            --secondChild;
        *CTL_vector_iterator_move(first, holeIndex).data = *CTL_vector_iterator_move(first, secondChild).data;
        holeIndex = secondChild;
        //下一个字节点位置
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len)
    {
        //右节点为空 将左节点移动到父节点位置
        *CTL_vector_iterator_move(first, holeIndex).data = *CTL_vector_iterator_move(first, secondChild - 1).data;
        holeIndex = secondChild - 1;
    }
    //重新调整堆 在合适的位置插入 value 及是 先前last-1的值
    //不能直接在holeIndex节点插入 因为holeIndex的父节点 可能小于value
    __push_heap(first, holeIndex, topIndex, value);
}

void CTL_sort_heap(CTL_vector_iterator *first, CTL_vector_iterator *last)
{
    CTL_vector_iterator w_last = *last;
    //通过循环出堆 进行排序 大堆 变成 从小到大排序
    while(CTL_vector_iterator_diff(&w_last, first) > 1)
    {
        CTL_pop_heap(first, &w_last);
        w_last = CTL_vector_iterator_move(&w_last, -1);
    }
}

void CTL_make_heap(CTL_vector_iterator *first, CTL_vector_iterator *last)
{
    ptrdiff_t len = CTL_vector_iterator_diff(last, first);
    //数据量 小于或等于2 直接返回 无需操作
    if (len < 2)
        return;

    //parent是第一个需重新排列的子树头部
    ptrdiff_t parent = (len - 2) / 2;

    while (parent >= 0)
    {
        //len 是防止越界的 如果子节点 大于 父节点 便进行交换
        __adjust_heap(first, parent, len, *CTL_vector_iterator_move(first, parent).data);
        parent--;
    }
}