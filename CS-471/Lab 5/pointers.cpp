#include <queue>
#include <iostream>
#include <vector>
#include <list>

using namespace std;

vector<int> sum2(const int &a, const int &b);


int main(int argc, char const *argv[])
{
    int x = 1;
    int *p = &x;

    cout << *p;

    //sum(1,2);

    vector<int> v = {1,2};
    sum2(1,2);
    
    return 0;

}

int sum(const int &x, const int &y) {
    return x+y;
}

vector<int> sum2(const int &a, const int &b) {
    vector<int> v;

    v.push_back(a);
    v.push_back(b);
    
    for (auto &&i : v)
    {
        cout << i;
    }

    return v;
}

