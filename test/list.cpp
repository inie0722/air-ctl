#include <time.h>

#include <iostream>
#include <list>
#include <algorithm>

#include <gtest/gtest.h>

extern "C"
{
#include "CTL_list.h"
#include "CTL_allocator.h"
}

using namespace std;

list<int> stl;
CTL_list ctl;

void test_new()
{
    srand((unsigned)time(NULL));
    CTL_list_new(&ctl, sizeof(int));
}

TEST(Modifiers, push_back)
{
    for (int i = 0; i < 200; i++)
    {
        stl.push_back(i);
        CTL_list_push_back(&ctl, &i);
    }

    CTL_list_iterator ctl_it;
    CTL_list_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_list_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Modifiers, push_front)
{
    for (int i = 0; i < 200; i++)
    {
        stl.push_front(i);
        CTL_list_push_front(&ctl, &i);
    }

    CTL_list_iterator ctl_it;
    CTL_list_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_list_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Modifiers, pop_back)
{
    for (int i = 0; i < 50; i++)
    {
        stl.pop_back();
        CTL_list_pop_back(&ctl);
    }

    CTL_list_iterator ctl_it;
    CTL_list_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_list_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Modifiers, pop_front)
{
    for (int i = 0; i < 50; i++)
    {
        stl.pop_front();
        CTL_list_pop_front(&ctl);
    }

    CTL_list_iterator ctl_it;
    CTL_list_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_list_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Modifiers, erase)
{
    for (int i = 0; i < 50; i++)
    {
        int pos = rand() % 50;

        CTL_list_iterator ctl_it;
        CTL_list_begin(&ctl, &ctl_it);
        auto stl_it = stl.begin();

        for (int i = 0; i < pos; i++)
        {
            stl_it++;
            CTL_list_iterator_move(&ctl_it, 1, &ctl_it);
        }

        stl.erase(stl_it);
        CTL_list_erase(&ctl, &ctl_it);
    }

    CTL_list_iterator ctl_it;
    CTL_list_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_list_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Modifiers, insert)
{
    for (int i = 0; i < 50; i++)
    {
        int pos = rand() % 50;

        CTL_list_iterator ctl_it;
        CTL_list_begin(&ctl, &ctl_it);
        auto stl_it = stl.begin();

        for (int i = 0; i < pos; i++)
        {
            stl_it++;
            CTL_list_iterator_move(&ctl_it, 1, &ctl_it);
        }

        CTL_list_insert(&ctl, &ctl_it, &i);
        stl.insert(stl_it, i);
    }

    CTL_list_iterator ctl_it;
    CTL_list_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_list_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Element_access, back)
{
    ASSERT_TRUE(stl.back() == *(int *)CTL_list_back(&ctl));
}

TEST(Element_access, front)
{
    ASSERT_TRUE(stl.front() == *(int *)CTL_list_front(&ctl));
}

TEST(Iterators, begin)
{
    CTL_list_iterator ctl_it;
    CTL_list_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_list_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(Iterators, end)
{
    CTL_list_iterator ctl_it;
    CTL_list_end(&ctl, &ctl_it);
    auto stl_it = stl.end();

    while (stl_it != stl.begin())
    {
        --stl_it;
        CTL_list_iterator_move(&ctl_it, -1, &ctl_it);
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
    }
}

TEST(Iterators, operator)
{
    CTL_list_iterator begin;
    CTL_list_begin(&ctl, &begin);

    CTL_list_iterator end;
    CTL_list_end(&ctl, &end);

    // ==
    ASSERT_TRUE(CTL_list_iterator_equal(&begin, &begin));
    ASSERT_FALSE(CTL_list_iterator_equal(&begin, &end));
}

TEST(Operations, splice)
{
    list<int> stl_sp;
    CTL_list ctl_sp;
    CTL_list_new(&ctl_sp, sizeof(int));

    for (int i = 0; i < 50; i++)
    {
        stl_sp.push_back(i);
        CTL_list_push_back(&ctl_sp, &i);
    }

    CTL_list_iterator begin;
    CTL_list_begin(&ctl, &begin);

    CTL_list_iterator sp_begin;
    CTL_list_begin(&ctl_sp, &sp_begin);

    CTL_list_iterator sp_end;
    CTL_list_end(&ctl_sp, &sp_end);

    CTL_list_splice(&ctl, &begin, &sp_begin, &sp_end);
    stl.splice(stl.begin(), stl_sp, stl_sp.begin(), stl_sp.end());

    CTL_list_iterator ctl_it;
    CTL_list_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_list_iterator_move(&ctl_it, 1, &ctl_it);
    }

    CTL_list_delete(&ctl_sp);
}

bool max(void *a, void *b)
{
    return *(int *)a > *(int *)b;
}

TEST(Operations, merge)
{
    stl.clear();
    CTL_list_clear(&ctl);

    for (int i = 0; i < 100; i += 2)
    {
        stl.push_back(i);
        CTL_list_push_back(&ctl, &i);
    }

    list<int> stl_mer;
    CTL_list ctl_mer;
    CTL_list_new(&ctl_mer, sizeof(int));

    for (int i = 0; i < 50; i += 2)
    {
        stl_mer.push_back(i);
        CTL_list_push_back(&ctl_mer, &i);
    }

    stl.merge(stl_mer);
    CTL_list_merge(&ctl, &ctl_mer, max);

    CTL_list_iterator ctl_it;
    CTL_list_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_list_iterator_move(&ctl_it, 1, &ctl_it);
    }

    CTL_list_delete(&ctl_mer);
}

TEST(Operations, sort)
{
    for (int i = 0; i < 100 * 10000; i++)
    {
        int val = rand();
        CTL_list_push_back(&ctl, &val);
        stl.push_back(val);
    }

    clock_t time = clock();
    CTL_list_sort(&ctl, max);
    cout << "CTL list sort: " << clock() - time << " us" << endl;

    time = clock();
    stl.sort();
    cout << "STL list sort: " << clock() - time << " us" << endl;

    CTL_list_iterator ctl_it;
    CTL_list_begin(&ctl, &ctl_it);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        CTL_list_iterator_move(&ctl_it, 1, &ctl_it);
    }
}

TEST(allocator, delete)
{
    ASSERT_TRUE(CTL_list_size(&ctl) == stl.size());
    CTL_list_delete(&ctl);
    ASSERT_TRUE(CTL_get_mem_size() == 0);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}