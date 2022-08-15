#include <iostream>
#include <array>

#include <gtest/gtest.h>

extern "C"
{
#include "air/ctl/array.h"
}

using namespace std;

CTL_array ctl;
array<int, 100> stl;

int *Ptr_array;

void test_new()
{
    Ptr_array = (int*)malloc(100 * sizeof(100));
    CTL_array_init(&ctl, Ptr_array, Ptr_array + 100, sizeof(int));

    for (size_t i = 0; i < 100; i++)
    {
        Ptr_array[i] = i;
        stl[i] = i;
    }
}

TEST(Element_access, at)
{
    for (size_t i = 0; i < 100; i++)
    {
        CTL_array_iterator at;
        CTL_array_begin(&ctl, &at);
        CTL_array_iterator_move(&at, i, &at);

        ASSERT_TRUE(stl[i] == *(int *)at.data);
    }
}

TEST(Element_access, back)
{
    ASSERT_TRUE(stl.back() == *(int *)CTL_array_back(&ctl));
}

TEST(Element_access, front)
{
    ASSERT_TRUE(stl.front() == *(int *)CTL_array_front(&ctl));
}

TEST(Iterators, begin)
{
    auto stl_it = stl.begin();
    CTL_array_iterator ctl_it;
    CTL_array_begin(&ctl, &ctl_it);

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_array_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Iterators, end)
{
    auto stl_it = stl.end();
    CTL_array_iterator ctl_it;
    CTL_array_end(&ctl, &ctl_it);

    while (stl_it != stl.begin())
    {
        --stl_it;
        CTL_array_iterator_move(&ctl_it, -1, &ctl_it);
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
    }
}

TEST(Iterators, operator)
{
    CTL_array_iterator begin;
    CTL_array_iterator end;
    CTL_array_begin(&ctl, &begin);
    CTL_array_end(&ctl, &end);

    // ==
    ASSERT_TRUE(CTL_array_iterator_equal(&begin, &begin));
    ASSERT_FALSE(CTL_array_iterator_equal(&begin, &end));

    // -
    ASSERT_TRUE(CTL_array_iterator_diff(&begin, &end) == stl.begin() - stl.end());
    ASSERT_TRUE(CTL_array_iterator_diff(&end, &begin) == stl.end() - stl.begin());

    // <
    ASSERT_TRUE(CTL_array_iterator_less(&begin, &end));
    ASSERT_FALSE(CTL_array_iterator_less(&end, &begin));
}

TEST(Capacity, size)
{
    ASSERT_TRUE(stl.size() == CTL_array_size(&ctl));
}

TEST(Capacity, capacity)
{
    ASSERT_TRUE(stl.max_size() == CTL_array_capacity(&ctl));
    free(Ptr_array);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}