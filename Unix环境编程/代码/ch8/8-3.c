#include "apue.h"
int globvar = 6;
int main(void)
{
    int var = 88;
    pid_t pid;
    printf("before vfork\n"); 
    if ((pid = vfork()) < 0)
        err_sys("fork error");
    else if(pid == 0)
    {
            globvar++;
            var++;
            _exit(0);
    }
    printf("pid = %ld, glob = %d, var = %d\n",(long)getpid(),globvar,var);
    exit(0);
}