#include "apue.h"

extern char **environ;
int main(int argc, char *argv[])
{
    int i;
    for(i = 0;environ[i]!= NULL;++i)
    {
        printf("%s\n",environ[i]);
    }
    exit(0);
}