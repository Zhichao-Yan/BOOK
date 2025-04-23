#include "partial.hpp"

int main()
{
    Data<int,double> a(1,2.0);
    a.display();
    Data<double,char> b(2.0,'A');
    b.display();
    return 0;
}