#include <iostream>

using namespace std;

extern "C"
{
#include "CTL_hash_map.h"
#include "CTL_tree_map.h"
}

bool equal(const void *a, const void *b)
{
    return *(int *)a == *(int *)b;
}

bool more(const void *a, const void *b)
{
    return *(int *)a > *(int *)b;
}

int main(void)
{
    CTL_hash_map hash;
    CTL_hash_map_new(&hash, sizeof(int), sizeof(int));

    CTL_tree_map tree;

    CTL_key key;
    key.size = sizeof(int);
    key.equal = equal;
    key.more = more;
    CTL_tree_map_new(&tree, &key, sizeof(int));

    clock_t time = clock();

    for (int i = 0; i < 1000000; i++)
    {
        int data = -i;
        CTL_tree_map_insert(&tree, &i, &data);
    }
    cout << "CTL tree insert: " << clock() - time << " us" << endl;

    time = clock();
    for (int i = 0; i < 1000000; i++)
    {
        int data = -i;
        CTL_hash_map_insert(&hash, &i, &data);
    }
    cout << "CTL hash insert: " << clock() - time << " us" << endl;

    time = clock();
    for (int i = 0; i < 100000; i++)
    {
        CTL_tree_map_iterator it;
        CTL_tree_map_find(&tree, &i, &it);
    }
    cout << "CTL tree find: " << clock() - time << " us" << endl;

    time = clock();
    for (int i = 0; i < 100000; i++)
    {
        CTL_hash_map_iterator it;
        CTL_hash_map_find(&hash, &i, &it);
    }
    cout << "CTL hash find: " << clock() - time << " us" << endl;
}