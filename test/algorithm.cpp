#include <iostream>
#include <vector>
#include <algorithm>

extern "C"
{
#include "../src/vector/CTL_vector.h"
#include "../src/algorithm/CTL_algorithm.h"

#include "../src/vector/CTL_vector.c"
#include "../src/allocator/CTL_allocator.c"
}

using namespace std;

int main(void)
{
    vector<int> stl_1, stl_2;
    CTL_vector ctl_1, ctl_2;
    CTL_vector_new(&ctl_1, 100);
    CTL_vector_new(&ctl_2, 100);

    for (size_t i = 0; i < 10; i++)
    {
        stl_1.push_back(i);
        stl_2.push_back(i);
        CTL_vector_push_back(&ctl_1, i);
        CTL_vector_push_back(&ctl_2, i);
    }
    
    copy(stl_1.begin(), stl_1.begin() + 3, stl_2.begin()+3);

    CTL_copy(vector, vector, ctl_1.begin, CTL_vector_iterator_move(&ctl_1.begin, 3, false), CTL_vector_iterator_move(&ctl_2.begin, 3, false));
    
    auto ctl_begin = ctl_2.begin;

    for (auto begin = stl_2.begin(); begin != stl_2.end(); ++begin)
    {
        cout<<*begin<< *ctl_begin.data <<endl;
        ctl_begin = CTL_vector_iterator_move(&ctl_begin, 1, false);
    }
    
    return 0;
}