#include <iostream>
#include <gtest/gtest.h>
#include <vector>

#include <time.h>

extern "C"
{
#include "CTL_vector.h"
}

using namespace std;

vector<int> stl;
CTL_vector ctl;

void test_new()
{
    srand((unsigned)time(NULL));
    CTL_vector_new(&ctl, 10, sizeof(int));
}

TEST(Modifiers, push_back)
{
    for (int i = 0; i < 150; i++)
    {
        stl.push_back(i);
        CTL_vector_push_back(&ctl, &i);
    }

    for (int i = 0; i < stl.size(); i++)
    {
        ASSERT_TRUE(stl[i] == ((int *)ctl.begin.data)[i]);
    }
}

TEST(Modifiers, pop_back)
{
    for (int i = 0; i < 50; i++)
    {
        stl.pop_back();
        CTL_vector_pop_back(&ctl);
    }

    for (int i = 0; i < stl.size(); i++)
    {
        ASSERT_TRUE(stl[i] == ((int *)ctl.begin.data)[i]);
    }
}

TEST(Modifiers, erase)
{
    for (int i = 0; i < 50; i++)
    {
        int pos = rand() % 50;
        stl.erase(stl.begin() + pos);
        auto at = CTL_vector_at(&ctl, pos);
        CTL_vector_erase(&ctl, &at);
    }

    for (int i = 0; i < stl.size(); i++)
    {
        ASSERT_TRUE(stl[i] == ((int *)ctl.begin.data)[i]);
    }
}

TEST(Modifiers, insert)
{
    for (int i = 0; i < 50; i++)
    {
        int pos = rand() % 50;
        stl.insert(stl.begin() + pos, i);
        auto at = CTL_vector_at(&ctl, pos);
        CTL_vector_insert(&ctl, &at, &i);
    }

    for (int i = 0; i < stl.size(); i++)
    {
        ASSERT_TRUE(stl[i] == ((int *)ctl.begin.data)[i]);
    }
}

TEST(Element_access, at)
{
    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(stl[i] == *(int *)CTL_vector_at(&ctl, i).data);
    }
}

TEST(Element_access, back)
{
    ASSERT_TRUE(stl.back() == *(int *)CTL_vector_back(&ctl));
}

TEST(Element_access, front)
{
    ASSERT_TRUE(stl.front() == *(int *)CTL_vector_front(&ctl));
}

TEST(Iterators, begin)
{
    auto stl_at = stl.begin();
    auto ctl_at = CTL_vector_begin(&ctl);

    while (stl_at != stl.end())
    {
        ASSERT_TRUE(*stl_at == *(int *)ctl_at.data);
        ++stl_at;
        ctl_at = CTL_vector_iterator_move(&ctl_at, 1);
    }
}

TEST(Iterators, end)
{
    auto stl_at = stl.end();
    auto ctl_at = CTL_vector_end(&ctl);

    while (stl_at != stl.begin())
    {
        --stl_at;
        ctl_at = CTL_vector_iterator_move(&ctl_at, -1);
        ASSERT_TRUE(*stl_at == *(int *)ctl_at.data);
    }
}

TEST(Iterators, operator)
{
    auto begin = CTL_vector_begin(&ctl);
    auto end = CTL_vector_end(&ctl);

    // ==
    ASSERT_TRUE(CTL_vector_iterator_equal(&begin, &begin));
    ASSERT_FALSE(CTL_vector_iterator_equal(&begin, &end));

    // -
    ASSERT_TRUE(CTL_vector_iterator_diff(&begin, &end) == stl.begin() - stl.end());
    ASSERT_TRUE(CTL_vector_iterator_diff(&end, &begin) == stl.end() - stl.begin());

    // >
    ASSERT_TRUE(CTL_vector_iterator_more(&end, &begin));
    ASSERT_FALSE(CTL_vector_iterator_more(&begin, &end));
}

TEST(Capacity, size)
{
    ASSERT_TRUE(stl.size() == CTL_vector_size(&ctl));
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