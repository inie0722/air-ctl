#include <time.h>

#include <iostream>

#include <gtest/gtest.h>

extern "C"
{
#include "CTL/hash_map.h"
}

using namespace std;

CTL_hash_map ctl;

void test_new()
{
    srand((unsigned)time(NULL));
    CTL_hash_map_new(&ctl, sizeof(int), sizeof(int));
}

TEST(Modifiers, insert)
{
    int data;
    for (int i = 0; i < 200; i++)
    {
        data = -i;
        CTL_hash_map_insert(&ctl, &i, &data);
    }

    int key = rand()%100;
    ASSERT_TRUE(-1 == CTL_hash_map_insert(&ctl, &key, &data));
    CTL_hash_map_iterator ctl_it;
    CTL_hash_map_find(&ctl, &key, &ctl_it);

    ASSERT_TRUE(-key == *(int *)ctl_it.data);
}

TEST(Modifiers, erase)
{   
    int key = rand()%100;
    CTL_hash_map_iterator ctl_it;
    CTL_hash_map_find(&ctl, &key, &ctl_it);
    CTL_hash_map_erase(&ctl, &ctl_it);

    ASSERT_TRUE(-1 == CTL_hash_map_find(&ctl, &key, &ctl_it));

    int data = -key;
    CTL_hash_map_insert(&ctl, &key, &data);
}

TEST(Iterators, begin)
{
    CTL_hash_map_iterator ctl_it, end;
    CTL_hash_map_begin(&ctl, &ctl_it);
    CTL_hash_map_end(&ctl, &end);


    int arr[200] = {0};
    while (!CTL_hash_map_iterator_equal(&ctl_it, &end))
    {
        arr[-*(int*)ctl_it.data]++;
        CTL_hash_map_iterator_next(&ctl_it, &ctl_it);
    }

    for (size_t i = 0; i < 200; i++)
    {
        ASSERT_TRUE(arr[i] == 1);
    }
}

TEST(Iterators, operator)
{
    CTL_hash_map_iterator begin;
    CTL_hash_map_begin(&ctl, &begin);

    CTL_hash_map_iterator end;
    CTL_hash_map_end(&ctl, &end);

    // ==
    ASSERT_TRUE(CTL_hash_map_iterator_equal(&begin, &begin));
    ASSERT_FALSE(CTL_hash_map_iterator_equal(&begin, &end));
}

TEST(allocator, delete)
{
    ASSERT_TRUE(CTL_hash_map_size(&ctl) == 200);
    CTL_hash_map_delete(&ctl);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}