#pragma once

#include<iostream>
using namespace std;

template<typename T>
class Test
{
public:
    Test()
    {
        cout << "construct a generic template class object" << endl;
    }
};
template<>
class Test<int>
{
public:
    Test()
    {
        cout << "construct a specialized template class object" << endl;
    }
};