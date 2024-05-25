/* $begin sigsuspend */
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
        // 开始之前要阻塞SIGCHLD信号
        Sigprocmask(SIG_BLOCK, &mask, &prev); /* Block SIGCHLD */
        if (Fork() == 0) /* Child */
            exit(0);

        /* Wait for SIGCHLD to be received */
        pid = 0;
        while (!pid) 
            Sigsuspend(&prev);  // 暂时取消阻塞SIGCHLD，来等待SIGCHLD
            // 如果从处理程序返回，那么sigsuspend会恢复阻塞SIGCHILD

        /* Optionally unblock SIGCHLD */
        // 后续我们可以根据情况选择是否继续阻塞SIGCHLD
        Sigprocmask(SIG_SETMASK, &prev, NULL); 

        /* Do some work after receiving SIGCHLD */
        printf(".");
    }
    exit(0);
}
/* $end sigsuspend */
