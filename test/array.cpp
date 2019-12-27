#include <gtest/gtest.h>

#include <iostream>
#include <array>

extern "C"
{
#include "CTL_array.h"
}

using namespace std;

CTL_array ctl;
array<int, 100> stl;

void test_new()
{
    int *array = new int[100];
    CTL_array_init(&ctl, array, array + 100, sizeof(int));

    for (size_t i = 0; i < 100; i++)
    {
        array[i] = i;
        stl[i] = i;
    }
}

TEST(Element_access, at)
{
    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(stl[i] == *(int *)CTL_array_at(&ctl, i).data);
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
    auto stl_at = stl.begin();
    auto ctl_at = CTL_array_begin(&ctl);

    while (stl_at != stl.end())
    {
        ASSERT_TRUE(*stl_at == *(int *)ctl_at.data);
        ++stl_at;
        ctl_at = CTL_array_iterator_move(&ctl_at, 1);
    }
}

TEST(Iterators, end)
{
    auto stl_at = stl.end();
    auto ctl_at = CTL_array_end(&ctl);

    while (stl_at != stl.begin())
    {
        --stl_at;
        ctl_at = CTL_array_iterator_move(&ctl_at, -1);
        ASSERT_TRUE(*stl_at == *(int *)ctl_at.data);
    }
}

TEST(Iterators, operator)
{
    auto begin = CTL_array_begin(&ctl);
    auto end = CTL_array_end(&ctl);

    // ==
    ASSERT_TRUE(CTL_array_iterator_equal(&begin, &begin));
    ASSERT_FALSE(CTL_array_iterator_equal(&begin, &end));

    // -
    ASSERT_TRUE(CTL_array_iterator_diff(&begin, &end) == stl.begin() - stl.end());
    ASSERT_TRUE(CTL_array_iterator_diff(&end, &begin) == stl.end() - stl.begin());

    // >
    ASSERT_TRUE(CTL_array_iterator_more(&end, &begin));
    ASSERT_FALSE(CTL_array_iterator_more(&begin, &end));
}

TEST(Capacity, size)
{
    ASSERT_TRUE(stl.size() == CTL_array_size(&ctl));
}

TEST(Capacity, capacity)
{
    ASSERT_TRUE(stl.max_size() == CTL_array_capacity(&ctl));
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}