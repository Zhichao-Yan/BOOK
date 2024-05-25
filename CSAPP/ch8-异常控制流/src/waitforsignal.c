/* $begin waitforsignal */
#include "csapp.h"

volatile sig_atomic_t pid;

void sigchld_handler(int s)
{
    int olderrno = errno;
    pid = Waitpid(-1, NULL, 0);
    errno = olderrno;
}

void sigint_handler(int s)
{
}

int main(int argc, char **argv) 
{
    sigset_t mask, prev;

    Signal(SIGCHLD, sigchld_handler);
    Signal(SIGINT, sigint_handler);
    Sigemptyset(&mask);
    Sigaddset(&mask, SIGCHLD);

    while (1) {
        // 阻塞SIGCHLD信号
        Sigprocmask(SIG_BLOCK, &mask, &prev); /* Block SIGCHLD */
        if (Fork() == 0) /* Child */
            exit(0);

        /* Parent */
        pid = 0; 
        // 取消阻塞SIGCHLD信号
        Sigprocmask(SIG_SETMASK, &prev, NULL); /* Unblock SIGCHLD */
        
        /* Wait for SIGCHLD to be received (wasteful) */
        // 等待pid变成非0，即收到SIGCHLD信号
        while (!pid) 
            ;
        // 循环终止才能进行其他工作
        /* Do some work after receiving SIGCHLD */
        printf(".");
    }
    exit(0);
}
/* $end waitforsignal */
