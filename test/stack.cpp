#include <time.h>

#include <iostream>
#include <stack>
#include <algorithm>

#include <gtest/gtest.h>

extern "C"
{
#include "air/ctl/stack.h"
}

using namespace std;

stack<int> stl;
CTL_stack ctl;

void test_new()
{
    srand((unsigned)time(NULL));

    CTL_stack_new(&ctl, CTL_deque_functions(), 18, sizeof(int));
}

TEST(Modifiers, push)
{
    for (int i = 0; i < 200; i++)
    {
        stl.push(i);
        CTL_stack_push(&ctl, &i);
        ASSERT_TRUE(stl.top() == *(int *)CTL_stack_top(&ctl));
    }
}


TEST(Modifiers, pop)
{
    for (int i = 0; i < 199; i++)
    {
        stl.pop();
        CTL_stack_pop(&ctl);
        ASSERT_TRUE(stl.top() == *(int *)CTL_stack_top(&ctl));
    }
}

TEST(Capacity, size)
{
    ASSERT_TRUE(stl.size() == CTL_stack_size(&ctl));
}

TEST(Capacity, empty)
{
    ASSERT_TRUE(stl.empty() == CTL_stack_empty(&ctl));
}

TEST(allocator, delete)
{
    CTL_stack_delete(&ctl);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}