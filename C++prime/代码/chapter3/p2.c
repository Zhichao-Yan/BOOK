#include <stdio.h>

int main()
{
    char a[5]={'A','B','C','D'}; 
    char (*p3)[3] = &a; 
    char (*p4)[3] = a;
    printf("p3:%p-p4:%p\n",&a,a);
    printf("p3+1:%p-p4+1%p\n",p3+1,p4+1);// p3+1 地址移动3个字节
    return 0;
}