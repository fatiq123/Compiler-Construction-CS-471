// 2021-CS-140

#include<chrono>
#include<iostream>
#include<stdio.h>
#include<list>
#include<thread>

using namespace std;

int thread11(const int& num = 100);
int thread22(const int& num = 260);


int main(int argc, char const *argv[])
{
    
    list<int> numbers = {100,260};


    thread t1(thread11);
    thread t2(thread22);

    t1.join();
    t2.join();

    thread t(thread11, thread22);
    t.join();

    int result = thread11(100) + thread22(260);
    cout << result;

    t1.detach();
    t2.detach();


    return 0;
}



list<int> thread1(list<int> num1) {
    list<int> l;
     for (list<int>::iterator it = num1.begin(); it != num1.end(); ++it)
    {
        cout << *it; 
        l.push_back[it];
    }
    for (int i = 0; i < num1.size(); i++)
    {
        l.pop_back(i);
    }
    
    
    l.push_back(num1);
    return l;
}


list<int> thread2(int num2) {
    list<int> l2;
    l2.push_back(num2);
    return l2;
}


int thread11(const int& num){
    return num;
}
int thread22(const int& num){
    return num;
}

