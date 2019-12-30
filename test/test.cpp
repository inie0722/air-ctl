#include <algorithm>
#include <iostream>
#include <deque>

using namespace std;

extern "C"
{
#include "CTL_heap.h"
#include "CTL_deque.h"
}



bool max(void* a, void* b)
{
    return *(int*)a < *(int*)b;
}

int main()
{
    CTL_deque ve;
    deque<int> stl;
    CTL_deque_new(&ve, 10, sizeof(int));

    for (int i = 0; i < 100; i++)
    {
        CTL_deque_push_back(&ve, &i);
        stl.push_back(i);
    }

    CTL_deque_iterator begin;
    CTL_deque_begin(&ve, &begin);

    CTL_deque_iterator end;
    CTL_deque_end(&ve, &end);

    CTL_make_heap(CTL_deque_functions(), (CTL_iterator*)&begin, (CTL_iterator*)&end, max);
    make_heap(stl.begin(), stl.end());

    for (int i = 0; i < 100; i++)
    {
        cout<< *(int*)begin.data <<" " << stl[i]<< endl;
        CTL_deque_iterator_move(&begin, 1, &begin);
    }
    
    return 0;
}
