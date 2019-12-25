#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>

extern "C"
{
#include "../src/vector/CTL_vector.h"
#include "../src/vector/CTL_vector.c"
#include "../src/heap/CTL_heap.c"
#include "../src/allocator/CTL_allocator.c"
}

using namespace std;

vector<int> stl;
CTL_vector ctl;

void test_new()
{
    srand((unsigned)time(NULL));
    CTL_vector_new(&ctl, 10);
}

TEST(push, Test)
{
    for (size_t i = 0; i < 100; i++)
    {
        stl.push_back(i);
        push_heap(stl.begin(), stl.end());

        CTL_vector_push_back(&ctl, i);
        CTL_push_heap(&ctl.begin, &ctl.end);
    }

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(stl[i] == ctl.begin.data[i]);
    }
}

TEST(sort, Test)
{
    sort_heap(stl.begin(), stl.end());
    CTL_sort_heap(&ctl.begin, &ctl.end);

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(stl[i] == ctl.begin.data[i]);
    }
}

TEST(make, Test)
{
    make_heap(stl.begin(), stl.end());
    CTL_make_heap(&ctl.begin, &ctl.end);

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(stl[i] == ctl.begin.data[i]);
    }
}

TEST(pop, Test)
{
    for (size_t i = 0; i < 100; i++)
    {
        pop_heap(stl.begin(), stl.end() - i);

        auto end = CTL_vector_at(&ctl, 100 - i);
        CTL_pop_heap(&ctl.begin, &end);
    }

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(stl[i] == ctl.begin.data[i]);
    }
}

TEST(allocator, delete)
{
    CTL_vector_delete(&ctl);
    ASSERT_TRUE(CTL_debug_mem == 0);
    ASSERT_TRUE(CTL_debug_mem_size == 0);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}