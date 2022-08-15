#include <time.h>

#include <iostream>
#include <deque>
#include <algorithm>

#include <gtest/gtest.h>

extern "C"
{
#include "air/ctl/deque.h"
}

using namespace std;

deque<int> stl;
CTL_deque ctl;

void test_new()
{
    srand((unsigned)time(NULL));
    CTL_deque_new(&ctl, 10, sizeof(int));
}

TEST(Modifiers, push_back)
{
    for (int i = 1; i < 100; i++)
    {
        stl.push_back(i);
        CTL_deque_push_back(&ctl, &i);
    }

    auto ctl_it = ctl.begin;
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_deque_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Modifiers, push_front)
{
    for (int i = 0; i < 100; i++)
    {
        stl.push_front(i);
        CTL_deque_push_front(&ctl, &i);
    }
    auto ctl_it = ctl.begin;
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_deque_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Modifiers, pop_back)
{
    for (int i = 0; i < 50; i++)
    {
        stl.pop_back();
        CTL_deque_pop_back(&ctl);
    }

    auto ctl_it = ctl.end;
    auto stl_it = stl.end();

    while (stl_it != stl.begin())
    {
        --stl_it;
        CTL_deque_iterator_move(&ctl_it, -1, &ctl_it);
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
    }
}

TEST(Modifiers, pop_front)
{

    for (int i = 0; i < 50; i++)
    {
        stl.pop_front();
        CTL_deque_pop_front(&ctl);
    }

    auto ctl_it = ctl.begin;
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_deque_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Modifiers, insert)
{
    for (int i = 0; i < 100; i++)
    {

        int pos = rand() % ctl.size;
        stl.insert(stl.begin() + pos, i);

        CTL_deque_iterator it;
        CTL_deque_begin(&ctl, &it);
        CTL_deque_iterator_move(&it, pos, &it);
        CTL_deque_insert(&ctl, &it, &i);
    }

    auto ctl_it = ctl.begin;
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_deque_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Modifiers, erase)
{
    for (int i = 0; i < 50; i++)
    {

        int pos = rand() % ctl.size;
        stl.erase(stl.begin() + pos);

        CTL_deque_iterator it;
        CTL_deque_begin(&ctl, &it);
        CTL_deque_iterator_move(&it, pos, &it);
        CTL_deque_erase(&ctl, &it);
    }

    auto ctl_it = ctl.begin;
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_deque_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Element_access, back)
{
    ASSERT_TRUE(stl.back() == *(int *)CTL_deque_back(&ctl));
}

TEST(Element_access, front)
{
    ASSERT_TRUE(stl.front() == *(int *)CTL_deque_front(&ctl));
}

TEST(Iterators, begin)
{
    auto stl_at = stl.begin();
    CTL_deque_iterator ctl_at;
    CTL_deque_begin(&ctl, &ctl_at);

    while (stl_at != stl.end())
    {
        ASSERT_TRUE(*stl_at == *(int *)ctl_at.data);
        ++stl_at;
        CTL_deque_iterator_move(&ctl_at, 1, &ctl_at);
    }
}

TEST(Iterators, end)
{
    auto stl_at = stl.end();
    CTL_deque_iterator ctl_at;
    CTL_deque_end(&ctl, &ctl_at);

    while (stl_at != stl.begin())
    {
        --stl_at;
        CTL_deque_iterator_move(&ctl_at, -1, &ctl_at);
        ASSERT_TRUE(*stl_at == *(int *)ctl_at.data);
    }
}

TEST(Iterators, operator)
{
    CTL_deque_iterator begin;
    CTL_deque_begin(&ctl, &begin);

    CTL_deque_iterator end;
    CTL_deque_end(&ctl, &end);

    // ==
    ASSERT_TRUE(CTL_deque_iterator_equal(&begin, &begin));
    ASSERT_FALSE(CTL_deque_iterator_equal(&begin, &end));

    // -
    ASSERT_TRUE(CTL_deque_iterator_diff(&begin, &end) == stl.begin() - stl.end());
    ASSERT_TRUE(CTL_deque_iterator_diff(&end, &begin) == stl.end() - stl.begin());

    // <
    ASSERT_TRUE(CTL_deque_iterator_less(&begin, &end));
    ASSERT_FALSE(CTL_deque_iterator_less(&end, &begin));
}

TEST(allocator, delete)
{
    CTL_deque_delete(&ctl);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}