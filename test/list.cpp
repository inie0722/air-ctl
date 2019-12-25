#include <iostream>
#include <gtest/gtest.h>
#include <list>
#include <algorithm>
#include <time.h>

extern "C"
{
#include "CTL_list.h"
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

    auto ctl_it = CTL_list_begin(&ctl);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1);
    }
}

TEST(Modifiers, push_front)
{
    for (int i = 0; i < 200; i++)
    {
        stl.push_front(i);
        CTL_list_push_front(&ctl, &i);
    }

    auto ctl_it = CTL_list_begin(&ctl);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1);
    }
}


TEST(Modifiers, pop_back)
{
    cout<<(void*)ctl.list->prior << " "<<(void*)ctl.list->next->prior<<endl;
    for (int i = 0; i < 50; i++)
    {
        stl.pop_back();
        CTL_list_pop_back(&ctl);
        cout<<(void*)ctl.list->prior << " "<<(void*)ctl.list->next->prior<<endl;
    }

    auto ctl_it = CTL_list_begin(&ctl);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1);
    }
}


TEST(Modifiers, pop_front)
{
    for (int i = 0; i < 50; i++)
    {
        stl.pop_front();
        CTL_list_pop_front(&ctl);
    }

    auto ctl_it = CTL_list_begin(&ctl);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1);
    }
}

TEST(Modifiers, erase)
{
    for (int i = 0; i < 50; i++)
    {
        int pos = rand() % 50;
        auto stl_it = stl.begin();
        auto ctl_it = CTL_list_begin(&ctl);

        for (int i = 0; i < pos; i++)
        {
            stl_it++;
            ctl_it = CTL_list_iterator_move(&ctl_it, 1);
        }

        stl.erase(stl_it);
        CTL_list_erase(&ctl, &ctl_it);
    }

    auto ctl_it = CTL_list_begin(&ctl);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1);
    }
}

TEST(Modifiers, insert)
{
    for (int i = 0; i < 50; i++)
    {
        int pos = rand() % 50;
        auto stl_it = stl.begin();
        auto ctl_it = CTL_list_begin(&ctl);

        for (int i = 0; i < pos; i++)
        {
            stl_it++;
            ctl_it = CTL_list_iterator_move(&ctl_it, 1);
        }

        CTL_list_insert(&ctl, &ctl_it, &i);
        stl.insert(stl_it, i);
    }

    auto ctl_it = CTL_list_begin(&ctl);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1);
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
    auto stl_at = stl.begin();
    auto ctl_at = CTL_list_begin(&ctl);

    while (stl_at != stl.end())
    {
        ASSERT_TRUE(*stl_at == *(int *)ctl_at.data);
        ++stl_at;
        ctl_at = CTL_list_iterator_move(&ctl_at, 1);
    }
}

TEST(Iterators, end)
{
    auto stl_at = stl.end();
    auto ctl_at = CTL_list_end(&ctl);

    while (stl_at != stl.begin())
    {
        --stl_at;
        ctl_at = CTL_list_iterator_move(&ctl_at, -1);
        ASSERT_TRUE(*stl_at == *(int *)ctl_at.data);
    }
}

TEST(Iterators, operator)
{
    auto begin = CTL_list_begin(&ctl);
    auto end = CTL_list_end(&ctl);

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

    auto begin = CTL_list_begin(&ctl);
    auto sp_begin = CTL_list_begin(&ctl_sp);
    auto sp_end = CTL_list_end(&ctl_sp);

    CTL_list_splice(&ctl, &begin, &sp_begin, &sp_end);
    stl.splice(stl.begin(), stl_sp, stl_sp.begin(), stl_sp.end());

    auto ctl_it = CTL_list_begin(&ctl);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1);
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

    auto ctl_it = CTL_list_begin(&ctl);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1);
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

    auto ctl_it = CTL_list_begin(&ctl);
    auto stl_it = stl.begin();

    while (stl_it != stl.end())
    {
        ASSERT_TRUE(*stl_it == *(int *)ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1);
    }
}

TEST(allocator, delete)
{
    ASSERT_TRUE(CTL_list_size(&ctl) == stl.size());
    CTL_list_delete(&ctl);
    ASSERT_TRUE(CTL_debug_mem_size == 0);
    ASSERT_TRUE(CTL_debug_mem == 0);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}