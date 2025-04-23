#include <stdio.h>
int main()
{
    char a[5] = {'a', 'b', 'c', 'd'};
    char(*p1)[5] = &a;
    char(*p2)[5] = (char(*)[5])a;
    printf("a=%p\n", a);//a[0]的地址
    printf("a=%c\n", a[0]);//a[0]的值
    printf("p1=%c\n", **p1);//a
    printf("p2=%c\n", **p2);//a
    printf("p1+1=%c\n", **(p1 + 1));//读取的字符为空'\0'
    printf("p2+1=%c\n", **(p2 + 1));//读取的字符为空'\0'
    return 0;
}
