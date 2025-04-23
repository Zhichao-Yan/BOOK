#include "partial.hpp"

int main()
{
    Bag<int> a;
    Bag<int*> b;
    int c = 100;
    a.add(c);
    a.print();
    b.add(&c);
    b.print();
    return 0;
}