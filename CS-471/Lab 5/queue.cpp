#include <queue>
#include <iostream>
#include <vector>
#include <list>

using namespace std;

int main(int argc, char const *argv[])
{

    queue<string> animals;

    animals.push("lion");
    animals.push("kangaroo");
    animals.push("zebra");

    // print_animals();

    list<int> numbers = {1, 2, 3, 4, 5};

    for (list<int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
        cout << *it; // Dereference the iterator to get the value
    }

    for (const int &num : numbers) {
        cout << num;
    }
    {
        /* code */
    }
    

    return 0;
}

void print_animals(queue<string> q)
{
    while (!q.empty())
    {
        cout << q.front() << "\n";
        q.pop();
    }
}