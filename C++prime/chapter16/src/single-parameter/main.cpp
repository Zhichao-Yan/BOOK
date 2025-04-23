#include "single.hpp"

int main()
{
    int m[MAXSIZE]{1,2,4,6,7};
    Matrix<int> a;
    a.SetMatrix(m);
    a.PrintMatrix();
    a.AddMatrix(m);
    a.PrintMatrix();
    return 0;
}