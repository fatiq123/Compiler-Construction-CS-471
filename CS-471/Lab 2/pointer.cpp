#include<iostream>
#include<stdio.h>
using namespace std;


struct Student
{
    private: string name;
    private: int age;
    public: int roll_no;

    private:
        void sum(int num1, int num2) {
            int num = num1 + num2;
        }
    public:
        void sum(double num1, double num2) {
            double num = num1 + num2;
        }
};

class Student
{
private:
    string name;
public:
    int age;
    int roll_no;
    
    Student(/* args */);
    ~Student();
};

Student::Student(/* args */)
{
}

Student::~Student()
{
}



int main() {
   
}