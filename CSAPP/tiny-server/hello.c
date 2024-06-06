#include <stdio.h>


void calculate(int a,int b)
{
    int c = a + b;
    printf("%d + %d = %d\n",a,b,c);
    return;
}

int iterate(int a,int b)
{
    int sum = 0;
    for(int i = a; i <= b; ++i)
    {
        sum += i;
    }
    return sum;
}

int main()
{
    printf("hello world\n");
    int v1 = iterate(1,100);
    int v2 = iterate(101,200);
    calculate(v1,v2);
    printf("goodbye\n");
    return 0;
}
