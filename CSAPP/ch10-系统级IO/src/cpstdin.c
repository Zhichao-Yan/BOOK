#include "csapp.h"

int main()
{
    char c;
    while(Read(STDIN_FILENO,&c,1) != 0)
        Write(STDOUT_FILENO,&c,1);
    return 0;
}