#include "csapp.h"

int main()
{
    int fd1, fd2;
    char c1, c2;
    fd1 = Open("foobar.txt",O_RDONLY,0);
    fd2 = Open("foobar.txt",O_RDONLY,0);
    Read(fd1,&c1,1);
    Read(fd2,&c2,1);
    printf("c1 = %c \n",c1); // c1 = f
    printf("c2 = %c \n",c2); // c2 = f
    Close(fd1);
    Close(fd2);
    exit(0);
}