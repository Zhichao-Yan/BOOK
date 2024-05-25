#include "csapp.h"
int main()
{
    int x = 1;
    char B[] = "father";
    if(Fork() == 0)
    {
        strcpy(B,"child");
        printf("%s: x = %d\n",B,++x);
    }  
    printf("%s: x = %d\n",B,--x);  
    exit(0);
}