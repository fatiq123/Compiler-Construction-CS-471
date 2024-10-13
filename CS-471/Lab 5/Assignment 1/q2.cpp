// 2021-CS-140

#include <iostream>
#include <stdio.h>
#include <vector>
#include <stack>
#include<iterator>

using namespace std;

stack<string> visited_webpages(stack<string> visited);

int main(int argc, char const *argv[])
{

    // vector<int> history;

    stack<string> pages;
    pages.push("1");
    pages.push("2");
    pages.push("3");
    pages.push("4");

    visited_webpages(pages);
    
}

stack<string> visited_webpages(stack<string> visited)
{

    stack<string> history;

    // history.push("visited page 1");
    // history.push("visited page 2");
    // history.push("visited page 3");
    // history.push("visited page 4");

    // string go_back = "go_back";
    bool go_back = false;

    while (!visited.empty())
    {
        for (int i = 0; i < visited.size(); i++)
        {
            if (!go_back)
            {
               
                //cout << "going back to page 2";
                cout << visited.top();
                visited.pop(); 
                
            }
            else
            {
                return visited;
            }
        }
    }

    // return visited;
}
