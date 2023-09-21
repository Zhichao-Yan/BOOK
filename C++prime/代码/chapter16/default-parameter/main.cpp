#include "default.hpp"

int main()
{
    D<int,double> d1(1,2.99,'$');
    d1.print();
    D<int,float,bool> d2(1,2.7f,true);
    d2.print();
    return 0;
}