#include <time.h>

#include <iostream>
#include <queue>
#include <algorithm>

#include <gtest/gtest.h>

extern "C"
{
#include "air/ctl/priority_queue.h"
}

using namespace std;

priority_queue<int> stl;
CTL_priority_queue ctl;

bool max(const void *left, const void *right)
{
    return *(const int *)left < *(const int *)right;
}

void test_new()
{
    srand((unsigned)time(NULL));

    CTL_priority_queue_new(&ctl, max, CTL_vector_functions(), 10, sizeof(int));
}

TEST(Modifiers, push)
{
    for (int i = 0; i < 200; i++)
    {
        stl.push(i);
        CTL_priority_queue_push(&ctl, &i);
        ASSERT_TRUE(stl.top() == *(int *)CTL_priority_queue_top(&ctl));
    }
}

TEST(Modifiers, pop)
{
    for (int i = 0; i < 199; i++)
    {
        stl.pop();
        CTL_priority_queue_pop(&ctl);
        ASSERT_TRUE(stl.top() == *(int *)CTL_priority_queue_top(&ctl));
    }
}

TEST(Capacity, size)
{
    ASSERT_TRUE(stl.size() == CTL_priority_queue_size(&ctl));
}

TEST(Capacity, empty)
{
    ASSERT_TRUE(stl.empty() == CTL_priority_queue_empty(&ctl));
}

TEST(allocator, delete)
{
    CTL_priority_queue_delete(&ctl);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}