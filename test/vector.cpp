#include <time.h>

#include <iostream>
#include <vector>

#include <gtest/gtest.h>

extern "C"
{
#include "CTL/vector.h"
#include "CTL/allocator.h"
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

    for (size_t i = 0; i < stl.size(); i++)
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

    for (size_t i = 0; i < stl.size(); i++)
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
        CTL_vector_iterator at;

        CTL_vector_begin(&ctl, &at);
        CTL_vector_iterator_move(&at, pos, &at);
        CTL_vector_erase(&ctl, &at);
    }

    for (size_t i = 0; i < stl.size(); i++)
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
        CTL_vector_iterator at;
        CTL_vector_begin(&ctl, &at);
        CTL_vector_iterator_move(&at, pos, &at);
        CTL_vector_insert(&ctl, &at, &i);
    }

    for (size_t i = 0; i < stl.size(); i++)
    {
        ASSERT_TRUE(stl[i] == ((int *)ctl.begin.data)[i]);
    }
}

TEST(Element_access, at)
{
    for (size_t i = 0; i < 100; i++)
    {
        CTL_vector_iterator at;
        CTL_vector_begin(&ctl, &at);
        CTL_vector_iterator_move(&at, i, &at);
        ASSERT_TRUE(stl[i] == *(int *)at.data);
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
    CTL_vector_iterator ctl_at;
    CTL_vector_begin(&ctl, &ctl_at);

    while (stl_at != stl.end())
    {
        ASSERT_TRUE(*stl_at == *(int *)ctl_at.data);
        ++stl_at;
        CTL_vector_iterator_move(&ctl_at, 1, &ctl_at);
    }
}

TEST(Iterators, end)
{
    auto stl_at = stl.end();
    CTL_vector_iterator ctl_at;
    CTL_vector_end(&ctl, &ctl_at);

    while (stl_at != stl.begin())
    {
        --stl_at;
        CTL_vector_iterator_move(&ctl_at, -1, &ctl_at);
        ASSERT_TRUE(*stl_at == *(int *)ctl_at.data);
    }
}

TEST(Iterators, operator)
{
    CTL_vector_iterator begin;
    CTL_vector_iterator end;
    CTL_vector_begin(&ctl, &begin);
    CTL_vector_end(&ctl, &end);

    // ==
    ASSERT_TRUE(CTL_vector_iterator_equal(&begin, &begin));
    ASSERT_FALSE(CTL_vector_iterator_equal(&begin, &end));

    // -
    ASSERT_TRUE(CTL_vector_iterator_diff(&begin, &end) == stl.begin() - stl.end());
    ASSERT_TRUE(CTL_vector_iterator_diff(&end, &begin) == stl.end() - stl.begin());

    // <
    ASSERT_TRUE(CTL_vector_iterator_less(&begin, &end));
    ASSERT_FALSE(CTL_vector_iterator_less(&end, &begin));
}

TEST(Capacity, size)
{
    ASSERT_TRUE(stl.size() == CTL_vector_size(&ctl));
}

TEST(allocator, delete)
{
    CTL_vector_delete(&ctl);
    ASSERT_TRUE(CTL_get_mem_size() == 0);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}