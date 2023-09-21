#pragma once
#include <iostream>
using namespace std;

template <typename T1,typename T2>
class Data
{
private:
    T1 a;
    T2 b;
public:
    Data(T1 t1, T2 t2):a(t1),b(t2)
    {
        cout << "construct an object of original template class" << endl;
    }
    void display()
    {
        cout << "original template class object's data:" << a << ',' << b << endl;
    }
    ~Data()
    {
        cout << "destruct an object of original template class" << endl;
    }
};

template<typename T>
class Data<T,char>
{
private:
    T a;
    char b;
public:
    Data(T t, char c):a(t),b(c)
    {
        cout << "construct an object of partial specialized class" << endl;
    }
    void display()
    {
        cout << "partial specialized class object's data:" << a << ',' << b << endl;
    }
    ~Data()
    {
        cout << "destruct an object of partial specialized class" << endl;
    }
};