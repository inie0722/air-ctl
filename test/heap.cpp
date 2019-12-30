#include <iostream>
#include <vector>
#include <algorithm>

#include <gtest/gtest.h>

extern "C"
{
#include "CTL_heap.h"
#include "CTL_vector.h"
#include "CTL_allocator.h"
}

using namespace std;

vector<size_t> stl;
CTL_vector ctl;

void test_new()
{
    srand((unsigned)time(NULL));
    CTL_vector_new(&ctl, 10, sizeof(size_t));
}

bool max(void *a, void *b)
{
    return *(size_t *)a < *(size_t *)b;
}

TEST(push, Test)
{
    for (size_t i = 0; i < 100; i++)
    {
        stl.push_back(i);
        push_heap(stl.begin(), stl.end());

        CTL_vector_push_back(&ctl, &i);

        CTL_vector_iterator begin;
        CTL_vector_begin(&ctl, &begin);

        CTL_vector_iterator end;
        CTL_vector_end(&ctl, &end);

        CTL_push_heap(CTL_vector_functions(), (CTL_iterator *)&begin, (CTL_iterator *)&end, max);
    }

    for (size_t i = 0; i < stl.size(); i++)
    {
        ASSERT_TRUE(stl[i] == *(size_t *)CTL_vector_at(&ctl, i));
    }
}

TEST(sort, Test)
{
    srand((unsigned)time(NULL));

    for (size_t i = 0; i < 100000; i++)
    {
        size_t val = rand();
        CTL_vector_push_back(&ctl, &val);
        stl.push_back(val);
    }

    CTL_vector_iterator begin;
    CTL_vector_begin(&ctl, &begin);

    CTL_vector_iterator end;
    CTL_vector_end(&ctl, &end);

    clock_t time = clock();
    CTL_sort_heap(CTL_vector_functions(), (CTL_iterator *)&begin, (CTL_iterator *)&end, max);
    cout << "CTL heap sort: " << clock() - time << " us" << endl;

    time = clock();
    sort_heap(stl.begin(), stl.end());
    cout << "STL heap sort: " << clock() - time << " us" << endl;

    for (size_t i = 0; i < stl.size(); i++)
    {
        ASSERT_TRUE(stl[i] == *(size_t *)CTL_vector_at(&ctl, i));
    }
}

TEST(make, Test)
{

    CTL_vector_iterator begin;
    CTL_vector_begin(&ctl, &begin);

    CTL_vector_iterator end;
    CTL_vector_end(&ctl, &end);

    clock_t time = clock();
    CTL_make_heap(CTL_vector_functions(), (CTL_iterator *)&begin, (CTL_iterator *)&end, max);
    cout << "CTL heap make: " << clock() - time << " us" << endl;

    time = clock();
    make_heap(stl.begin(), stl.end());
    cout << "STL heap make: " << clock() - time << " us" << endl;

    for (size_t i = 0; i < stl.size(); i++)
    {
        ASSERT_TRUE(stl[i] == *(size_t *)CTL_vector_at(&ctl, i));
    }
}

TEST(pop, Test)
{
    for (size_t i = 0; i < 100; i++)
    {
        pop_heap(stl.begin(), stl.end() - i);

        CTL_vector_iterator begin;
        CTL_vector_begin(&ctl, &begin);

        CTL_vector_iterator end;
        CTL_vector_end(&ctl, &end);

        CTL_vector_iterator_move(&begin, stl.size() - i, &end);
        CTL_pop_heap(CTL_vector_functions(), (CTL_iterator *)&begin, (CTL_iterator *)&end, max);
    }

    for (size_t i = 0; i < stl.size(); i++)
    {
        ASSERT_TRUE(stl[i] == *(size_t *)CTL_vector_at(&ctl, i));
    }
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