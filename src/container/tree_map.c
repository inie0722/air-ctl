#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "CTL/tree_map.h"
#include "CTL/allocator.h"

#define RED true

#define BLACK false

static inline __rb_tree_node *replace(const __rb_tree_node *x)
{
    //X是中间节点 就查找替换值
    if (x->left != NULL && x->right != NULL)
    {
        x = x->right;
        while (x->left != NULL)
            x = x->left;

        return (__rb_tree_node *)x;
    }
    //如果 有一个子节点就 返回子节点
    //如果 没有子节点就 返回NULL
    else if (x->left != NULL)
    {
        return x->left;
    }
    else
    {
        return x->right;
    }
}

static inline __rb_tree_node *__root(const CTL_tree_map *handle)
{
    return handle->header->parent;
}

static inline bool is_on_left(const __rb_tree_node *x)
{
    return x->parent->left == x;
}

static inline bool has_black_child(const __rb_tree_node *x)
{
    return (x->left == NULL || x->left->color == BLACK) && (x->right == NULL || x->right->color == BLACK);
}

static inline __rb_tree_node *__uncle(const __rb_tree_node *x)
{
    if (is_on_left(x->parent))
    {
        return x->parent->parent->right;
    }
    else
    {
        return x->parent->parent->left;
    }
}

static inline __rb_tree_node *__sibling(const __rb_tree_node *x)
{
    if (is_on_left(x))
    {
        return x->parent->right;
    }
    else
    {
        return x->parent->left;
    }
}

static void __clear(CTL_tree_map *handle, __rb_tree_node *n)
{
    if (n->left != NULL)
    {
        __clear(handle, n->left);
    }
    if (n->right != NULL)
    {
        __clear(handle, n->right);
    }

    CTL_deallocate(n->data, handle->T_size);
    CTL_deallocate(n->key, handle->key.size);
    CTL_deallocate(n, sizeof(__rb_tree_node));
}

//左旋操作
static void rotate_left(CTL_tree_map *handle, __rb_tree_node *x)
{
    __rb_tree_node *y = x->right;

    //X如果是根节点 设置Y成为新的根节点
    if (x == __root(handle))
    {
        handle->header->parent = y;
    }
    //否则设置X的父节点
    else if (is_on_left(x))
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }

    //如果Y的左子节点存在
    if (y->left != NULL)
    {
        y->left->parent = x;
    }

    //Y代替X
    y->parent = x->parent;
    x->right = y->left;

    //X成为Y的左子节点
    y->left = x;
    x->parent = y;
}

//右旋操作
static void rotate_right(CTL_tree_map *handle, __rb_tree_node *x)
{
    __rb_tree_node *y = x->left;

    //X如果是根节点 设置Y成为新的根节点
    if (x == __root(handle))
    {
        handle->header->parent = y;
    }
    //否则设置X的父节点
    else if (is_on_left(x))
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }

    //如果Y的右子节点存在
    if (y->right != NULL)
    {
        y->right->parent = x;
    }

    //Y代替X
    y->parent = x->parent;
    x->left = y->right;

    //X成为Y的左子节点
    y->right = x;
    x->parent = y;
}

static void fix_insert(CTL_tree_map *handle, __rb_tree_node *x)
{
    while (x != __root(handle) && x->parent->color == RED)
    {
        __rb_tree_node *parent = x->parent;
        __rb_tree_node *grandparent = parent->parent;
        __rb_tree_node *uncle = __uncle(x);

        //递归调整颜色
        if (uncle != NULL && uncle->color == RED)
        {
            parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED;

            x = grandparent;
        }
        //进行旋转和变色才可以
        else
        {
            if (is_on_left(parent))
            {
                //先把X 和 父节点 弄到一边
                if (!is_on_left(x))
                {
                    x = parent;
                    rotate_left(handle, x);
                }

                //先进行变色
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                //再进行旋转
                rotate_right(handle, x->parent->parent);
            }
            else
            {
                //和上面相反而已
                if (is_on_left(x))
                {
                    x = parent;
                    rotate_right(handle, x);
                }

                x->parent->color = BLACK;
                x->parent->parent->color = RED;

                rotate_left(handle, x->parent->parent);
            }
        }
    }

    __root(handle)->color = BLACK;
}

static void fix_erase(CTL_tree_map *handle, __rb_tree_node *x)
{
    while (x != __root(handle))
    {
        __rb_tree_node *sibling = __sibling(x);
        __rb_tree_node *parent = x->parent;

        //兄弟节点不存在 视为全部黑色处理
        if (sibling == NULL)
        {
            x = parent;
            continue;
        }
        else
        {
            if (sibling->color == BLACK)
            {
                //子节点 全是黑色
                if (has_black_child(sibling))
                {
                    //兄弟节点设置为红色
                    sibling->color = RED;

                    //父节点也是黑色 进行递归处理
                    if (parent->color == BLACK)
                    {
                        x = parent;
                        continue;
                    }
                    else
                    {
                        //如果父节点是红色 交互颜色即可
                        parent->color = BLACK;
                    }
                }
                //兄弟节点拥有一个或两个红色子节点 兄弟节点必定是黑色
                else
                {
                    //兄弟节点在左边
                    if (is_on_left(sibling))
                    {
                        //红色子节点也在左边
                        if (sibling->left != NULL && sibling->left->color == RED)
                        {
                            //兄弟节点代替父节点
                            sibling->color = parent->color;
                            //左字节点代替兄弟节点 兄弟节点
                            sibling->left->color = BLACK;
                            //以父节点右旋
                            rotate_right(handle, parent);
                        }
                        else
                        {
                            //让其右子节点代替父节点
                            sibling->right->color = parent->color;
                            //先让其右子节点代替兄弟节点
                            rotate_left(handle, sibling);
                            //再进行右旋
                            rotate_right(handle, parent);
                        }
                    }
                    //兄弟节点在右边
                    else
                    {
                        //红色子节点在左边
                        if (sibling->left != NULL && sibling->left->color == RED)
                        {
                            //子节点取代父节点
                            sibling->left->color = parent->color;
                            rotate_right(handle, sibling);
                            rotate_left(handle, parent);
                        }
                        else
                        {
                            //兄弟节点取代父节点
                            sibling->color = parent->color;
                            sibling->right->color = BLACK;
                            rotate_left(handle, parent);
                        }
                    }

                    //父节点此时已经旋转到被删除一边了
                    //因为删除了一个黑色节点 这里就把父节点移动代替被删除节点 用来保持平衡
                    parent->color = BLACK;
                }
            }
            //兄弟节点是红色 意味着兄弟节点必定拥有两个黑色子节点 不然无法保持平衡
            else
            {
                //父节点和兄弟节点交换颜色 再进行旋转
                //情况就转为 兄黑了
                parent->color = RED;
                sibling->color = BLACK;
                //兄弟节点在左边
                if (is_on_left(sibling))
                {
                    rotate_right(handle, parent);
                }
                else
                {
                    rotate_left(handle, parent);
                }
                continue;
            }
        }
        break;
    }
}

void CTL_tree_map_new(CTL_tree_map *handle, CTL_tree_map_key *key, const size_t T_size)
{
    handle->header = (__rb_tree_node *)CTL_allocate(sizeof(__rb_tree_node));
    handle->header->parent = NULL;
    handle->header->left = handle->header;
    handle->header->right = handle->header;
    handle->header->color = RED;

    handle->T_size = T_size;
    handle->key = *key;
    handle->size = 0;
}

void CTL_tree_map_clear(CTL_tree_map *handle)
{
    __clear(handle, __root(handle));
    handle->header->parent = NULL;
    handle->header->left = handle->header;
    handle->header->right = handle->header;
    handle->size = 0;
}

void CTL_tree_map_delete(CTL_tree_map *handle)
{
    CTL_tree_map_clear(handle);
    CTL_deallocate(handle->header, sizeof(__rb_tree_node));
}

void CTL_tree_map_begin(const CTL_tree_map *handle, CTL_tree_map_iterator *res)
{
    res->T_size = handle->T_size;
    res->data = handle->header->left->data;
    res->node = handle->header->left;
}

void CTL_tree_map_end(const CTL_tree_map *handle, CTL_tree_map_iterator *res)
{
    res->T_size = handle->T_size;
    res->data = NULL;
    res->node = handle->header;
}

void CTL_tree_map_insert(CTL_tree_map *handle, void *key, void *element)
{
    __rb_tree_node *i = __root(handle);
    __rb_tree_node *parent = handle->header;

    //查找合适的插入点
    while (i != NULL)
    {
        parent = i;
        i = (handle->key.less(i->key, key) ? i->left : i->right);
    }

    __rb_tree_node *new_node = (__rb_tree_node *)CTL_allocate(sizeof(__rb_tree_node));
    new_node->data = (char *)CTL_allocate(handle->T_size);
    new_node->key = (char *)CTL_allocate(handle->key.size);
    //设置节点
    new_node->parent = parent;
    memcpy(new_node->key, key, handle->key.size);
    memcpy(new_node->data, element, handle->T_size);
    new_node->left = NULL;
    new_node->right = NULL;
    //新节点 默认 都是红色
    new_node->color = RED;

    if (parent == handle->header)
    {
        handle->header->parent = new_node;
        handle->header->left = new_node;
        handle->header->right = new_node;
    }
    else if (handle->key.less(parent->key, key))
    {
        parent->left = new_node;

        //如果data是最小的
        if (handle->header->left == parent)
        {
            handle->header->left = new_node;
        }
    }
    else
    {
        parent->right = new_node;

        //如果data是最大的
        if (handle->header->right == parent)
        {
            handle->header->right = new_node;
        }
    }

    if (parent->color == RED)
    {
        fix_insert(handle, new_node);
    }
    ++handle->size;
}

void CTL_tree_map_find(const CTL_tree_map *handle, const void *key, CTL_tree_map_iterator *res)
{
    __rb_tree_node *t = handle->header->parent;
    __rb_tree_node *last = NULL; //最后一个不小于key的节点

    while (t != NULL)
    {
        if (!handle->key.less(key, t->key))
        {
            last = t;
            t = t->left;
        }
        else
        {
            t = t->right;
        }
    }

    if (last == NULL || !handle->key.equal(key, last->key))
    {
        res->data = NULL;
    }
    else
    {
        res->data = last->data;
        res->node = last;
        res->T_size = handle->T_size;
    }
}

void CTL_tree_map_erase(CTL_tree_map *handle, CTL_tree_map_iterator *iterator)
{
    __rb_tree_node *v = iterator->node;

    //待删除节点的 替换节点

    __rb_tree_node *u = replace(v);
    if (v->left != NULL && v->right != NULL)
    {
        memcpy(v->data, u->data, handle->T_size);
        memcpy(v->key, u->key, handle->key.size);

        v = u;
        u = u->left != NULL ? u->left : u->right;
    }

    //判断下 V和U 是否都为黑色
    bool double_black = (u == NULL || u->color == BLACK) && v->color == BLACK;

    //无可替换节点
    if (u == NULL)
    {
        //free(u)
        u = v;

        //删除的是根节点 而且红黑树 还仅有根节点一个节点
        if (v == __root(handle))
        {
            handle->header->parent = NULL;
            handle->header->left = handle->header;
            handle->header->right = handle->header;
        }
        else
        {
            //删除黑色节点 需要特别 处理 NULL也视为黑色
            if (double_black)
            {
                fix_erase(handle, v);
            }

            if (is_on_left(v))
            {
                v->parent->left = NULL;
            }
            else
            {
                v->parent->right = NULL;
            }
        }
    }
    else
    {
        memcpy(v->data, u->data, handle->T_size);
        memcpy(v->key, u->key, handle->key.size);

        //删除黑色节点 需要特别 处理
        if (double_black)
        {
            fix_erase(handle, u);
        }
        else
        {
            v->color = BLACK;

            if (is_on_left(u))
            {
                u->parent->left = NULL;
            }
            else
            {
                u->parent->right = NULL;
            }
        }
    }

    //更新头节点信息
    //如果是最小值
    if (u == handle->header->left)
    {
        handle->header->left = u->parent;
    }
    //如果是最大值
    else if (u == handle->header->right)
    {
        handle->header->right = u->parent;
    }

    CTL_deallocate(u->data, handle->T_size);
    CTL_deallocate(u->key, handle->key.size);
    CTL_deallocate(u, sizeof(__rb_tree_node));

    --handle->size;
}

void CTL_tree_map_iterator_move(const CTL_tree_map_iterator *handle, const ptrdiff_t n, CTL_tree_map_iterator *res)
{
    __rb_tree_node *node = handle->node;

    //++操作
    if (n > 0)
    {
        for (int i = 0; i < n; ++i)
        {
            //非叶子节点
            if (handle->node->right)
            {
                node = node->right;
                while (node->left)
                {
                    node = node->left;
                }
            }
            //叶子节点
            else
            {
                __rb_tree_node *parent = node->parent;
                while (node == parent->right)
                {
                    node = parent;
                    parent = parent->parent;
                }

                //如果root节点无右节点 便会触发else 即返回root节点
                if (node->right != parent)
                    node = parent;
            }
        }
    }
    //--操作
    else
    {
        for (int i = 0; i > n; --i)
        {
            //如果node 是头结点 及end时
            if (node->color == RED && node->parent->parent == node)
            {
                node = node->right;
            }
            //非叶子节点
            else if (node->left != NULL)
            {
                __rb_tree_node *parent = node->left;
                while (parent->right)
                {
                    parent = parent->right;
                }
                node = parent;
            }
            //叶子节点
            else
            {
                __rb_tree_node *parent = node->parent;
                while (node == parent->left)
                {
                    node = parent;
                    parent = parent->parent;
                }
                node = parent;
            }
        }
    }

    res->node = node;
    res->data = node->data;
}

bool CTL_tree_map_iterator_equal(const CTL_tree_map_iterator *left, const CTL_tree_map_iterator *right)
{
    return left->node == right->node;
}

size_t CTL_tree_map_size(const CTL_tree_map *handle)
{
    return handle->size;
}

bool CTL_tree_map_empty(const CTL_tree_map *handle)
{
    return !__root(handle);
}