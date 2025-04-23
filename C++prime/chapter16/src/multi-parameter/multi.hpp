#pragma once
#include <iostream>
using namespace std;

template<typename T1,typename T2>
class A
{
private:
    T1 a;
    T2 b;
public:
    A(T1 t1,T2 t2):a(t1),b(t2){}
    void display()
    {
        cout << "values:" << a << ',' << b << endl;
    }
};