#include "csapp.h"

void initjobs()
{
}

/* addjob和deletejob之间存竞争同步错误 */
void addjob(int pid)
{
}

void deletejob(int pid)
{
}

/* $begin procmask1 */
/* WARNING: This code is buggy! */
void handler(int sig)
{
    int olderrno = errno;
    sigset_t mask_all, prev_all;
    pid_t pid;

    Sigfillset(&mask_all);
    while ((pid = waitpid(-1, NULL, 0)) > 0) { /* Reap a zombie child */
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        deletejob(pid); /* Delete the child from the job list */
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
    if (errno != ECHILD)
        Sio_error("waitpid error");
    errno = olderrno;
}
    
int main(int argc, char **argv)
{
    int pid;
    sigset_t mask_all, prev_all;

    Sigfillset(&mask_all);
    Signal(SIGCHLD, handler);
    initjobs(); /* Initialize the job list */

    while (1) {
        if ((pid = Fork()) == 0) { /* Child process */
            Execve("/bin/date", argv, NULL);
        }
        // 问题：在还没有运行到下面之前，子进程就终止了，然后就去执行SIGCHLD处理程序
        // 还没addjob，就要deletejob（啥也不干）
        // 父进程运行到下面，把不存在的子进程加入到作业列表
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all); /* Parent process */  
        addjob(pid);  /* Add the child to the job list */
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);    
    }
    exit(0);
}
/* $end procmask1 */
