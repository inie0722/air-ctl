#include <time.h>

#include <iostream>
#include <map>

#include <gtest/gtest.h>

extern "C"
{
#include "CTL_tree_map.h"
#include "CTL_allocator.h"
}

using namespace std;

multimap<int, int> stl;
CTL_tree_map ctl;

bool equal(const void *a, const void *b)
{
    return *(int *)a == *(int *)b;
}

bool more(const void *a, const void *b)
{
    return *(int *)a > *(int *)b;
}

void test_new()
{
    srand((unsigned)time(NULL));
    CTL_key key;
    key.size = sizeof(int);
    key.equal = equal;
    key.more = more;
    CTL_tree_map_new(&ctl, &key, sizeof(int));
}

TEST(Modifiers, insert)
{
    for (int i = 0; i < 200; i++)
    {
        int key = rand() % 100;
        stl.insert(make_pair(key, i));
        CTL_tree_map_insert(&ctl, &key, &i);
    }

    CTL_tree_map_iterator ctl_it;
    CTL_tree_map_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(stl_it->second == *(int *)ctl_it.data);
        ++stl_it;
        CTL_tree_map_iterator_move(&ctl_it, 1, &ctl_it);
    }
    cout << endl;
}

TEST(Modifiers, erase)
{
    for (int i = 0; i < 100; i++)
    {
        int key = rand() % 100;

        CTL_tree_map_iterator ctl_it;
        CTL_tree_map_find(&ctl, &key, &ctl_it);

        auto stl_it = stl.find(key);
        if (stl_it->first == key)
        {
            CTL_tree_map_erase(&ctl, &ctl_it);
            stl.erase(stl_it);
        }
    }

    CTL_tree_map_iterator ctl_it;
    CTL_tree_map_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(stl_it->second == *(int *)ctl_it.data);
        ++stl_it;
        CTL_tree_map_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Iterators, begin)
{
    CTL_tree_map_iterator ctl_it;
    CTL_tree_map_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(stl_it->second == *(int *)ctl_it.data);
        ++stl_it;
        CTL_tree_map_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Iterators, end)
{
    CTL_tree_map_iterator ctl_it;
    CTL_tree_map_end(&ctl, &ctl_it);
    auto stl_it = stl.end();

    while (stl_it != stl.begin())
    {
        --stl_it;
        CTL_tree_map_iterator_move(&ctl_it, -1, &ctl_it);
        ASSERT_TRUE(stl_it->second == *(int *)ctl_it.data);
    }
}

TEST(Iterators, operator)
{
    CTL_tree_map_iterator begin;
    CTL_tree_map_begin(&ctl, &begin);

    CTL_tree_map_iterator end;
    CTL_tree_map_end(&ctl, &end);

    // ==
    ASSERT_TRUE(CTL_tree_map_iterator_equal(&begin, &begin));
    ASSERT_FALSE(CTL_tree_map_iterator_equal(&begin, &end));
}

TEST(allocator, delete)
{
    ASSERT_TRUE(CTL_tree_map_size(&ctl) == stl.size());
    CTL_tree_map_delete(&ctl);
    ASSERT_TRUE(CTL_get_mem_size() == 0);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}