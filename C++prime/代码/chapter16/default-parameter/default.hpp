#pragma once
#include <iostream>
using namespace std;

template<typename X,typename Y,typename Z = char>
class D{
private:
    X x;
    Y y;
    Z z;
public:
    D(X a,Y b,Z c):x(a),y(b),z(c){}
    void print()
    {
        cout << "x = " << x << endl;
        cout << "y = " << y << endl;
        cout << "z = " << z << endl;
    }
};