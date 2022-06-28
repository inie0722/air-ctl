#include <time.h>

#include <iostream>
#include <queue>
#include <algorithm>

#include <gtest/gtest.h>

extern "C"
{
#include "CTL/queue.h"
#include "CTL/allocator.h"
}

using namespace std;

queue<int> stl;
CTL_queue ctl;

void test_new()
{
    srand((unsigned)time(NULL));

    CTL_queue_new(&ctl, CTL_list_functions(), 0, sizeof(int));
}

TEST(Modifiers, push)
{
    for (int i = 0; i < 200; i++)
    {
        stl.push(i);
        CTL_queue_push(&ctl, &i);
        ASSERT_TRUE(stl.front() == *(int *)CTL_queue_front(&ctl));
        ASSERT_TRUE(stl.back() == *(int *)CTL_queue_back(&ctl));
    }
}


TEST(Modifiers, pop)
{
    for (int i = 0; i < 199; i++)
    {
        stl.pop();
        CTL_queue_pop(&ctl);
        ASSERT_TRUE(stl.front() == *(int *)CTL_queue_front(&ctl));
        ASSERT_TRUE(stl.back() == *(int *)CTL_queue_back(&ctl));
    }
}

TEST(Capacity, size)
{
    ASSERT_TRUE(stl.size() == CTL_queue_size(&ctl));
}

TEST(Capacity, empty)
{
    ASSERT_TRUE(stl.empty() == CTL_queue_empty(&ctl));
}

TEST(allocator, delete)
{
    CTL_queue_delete(&ctl);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}