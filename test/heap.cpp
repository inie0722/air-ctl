#include <iostream>
#include <vector>
#include <algorithm>

#include <gtest/gtest.h>

extern "C"
{
#include "CTL/heap.h"
#include "CTL/vector.h"
#include "CTL/allocator.h"
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

TEST(Algorithm, sort)
{
    srand((unsigned)time(NULL));

    for (size_t i = 0; i < 1000000; i++)
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