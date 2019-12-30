#include <algorithm>
#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

extern "C"
{
#include "CTL_heap.h"
#include "CTL_vector.h"
}



bool max(void* a, void* b)
{
    return *(int*)a < *(int*)b;
}

int main()
{
    srand((unsigned)time(NULL));
    CTL_vector ve;
    vector<int> stl;
    CTL_vector_new(&ve, 512, sizeof(int));

    for (int i = 0; i < 10000; i++)
    {
        int val = rand();
        CTL_vector_push_back(&ve, &val);
        stl.push_back(val);
    }

    CTL_vector_iterator begin;
    CTL_vector_begin(&ve, &begin);

    CTL_vector_iterator end;
    CTL_vector_end(&ve, &end);

    clock_t time = clock();
    CTL_sort_heap(CTL_vector_functions(), (CTL_iterator*)&begin, (CTL_iterator*)&end, max);
    cout << "CTL heap sort: " << clock() - time << " us" << endl;

    time = clock();
    sort_heap(stl.begin(), stl.end());
    cout << "STL heap sort: " << clock() - time << " us" << endl;
    
    return 0;
}
