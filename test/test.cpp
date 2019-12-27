#include <stdio.h>
#include <deque>

//#include "CTL_container.h"
//#include "CTL_array.h"

using namespace std;

int main()
{
    deque<int> ve;
    ve.push_back(66);
    ve.push_back(66);
    ve.push_back(66);
    ve.push_back(66);
    ve.push_back(66);
    printf("%d\n", ve.capacity());
    return 0;
}