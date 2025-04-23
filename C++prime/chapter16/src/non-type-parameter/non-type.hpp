#pragma once
#include <iostream>
using namespace std;

template<typename T,size_t size>
class A
{
private:
    T array[size];
public:
    void insert();
    void display() const;
};

template<typename T,size_t size>
void A<T,size>::insert()
{
    int i = 1;
    for(size_t j = 0; j < size; ++j)
    {
        array[j] = i++;
    }
}
template<typename T,size_t size>
void A<T,size>::display() const
{
    for(size_t j = 0; j < size; ++j)
    {
        cout << array[j] << ' ';
    }
    cout << endl;
}
