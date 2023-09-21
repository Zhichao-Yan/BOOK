#pragma once
#include <iostream>
using namespace std;
#define MAXSIZE 5

template<typename T>
class Matrix
{
private:
    T matrix[MAXSIZE];
    size_t size;
public:
    Matrix();
    void PrintMatrix() const;
    void SetMatrix(T array[]);
    void AddMatrix(T array[]);
};

template<typename T>
Matrix<T>::Matrix():size(MAXSIZE){}

template<typename T>
void Matrix<T>::PrintMatrix() const
{
    for(size_t i = 0; i < size; ++i)
    {
        cout << matrix[i] << ' ';
    }
    cout << endl;
}

template<typename T>
void Matrix<T>::SetMatrix(T array[])
{
    for(size_t i = 0; i < size; ++i)
    {
        matrix[i] = array[i];
    }
}

template<typename T>
void Matrix<T>::AddMatrix(T array[])
{
    for(size_t i = 0; i < size; ++i)
    {
        matrix[i] += array[i];
    }
}