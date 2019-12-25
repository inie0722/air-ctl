#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;
int main(void)
{
    vector<int>l;
    l.push_back(99);
    l.push_back(98);
    l.pop_back();
    cout<<*l.end()<<endl;
    return 0;
}