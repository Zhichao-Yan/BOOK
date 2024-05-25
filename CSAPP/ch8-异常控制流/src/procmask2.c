#include "csapp.h"

void initjobs()
{
}

void addjob(int pid)
{
}

void deletejob(int pid)
{
}

/* $begin procmask2 */
void handler(int sig)
{
    int olderrno = errno;
    sigset_t mask_all, prev_all;
    pid_t pid;

    Sigfillset(&mask_all);
    while ((pid = waitpid(-1, NULL, 0)) > 0) { /* Reap a zombie child */
        // 访问全局数据结构（作业列表）为了安全要阻塞所有信号
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        deletejob(pid); /* Delete the child from the job list */
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
    if (errno != ECHILD)
        Sio_error("waitpid error");
    errno = olderrno;
}

// 使用信号机制Sigprocmask同步进程，在deletejob之前必然已经addjob
int main(int argc, char **argv)
{
    int pid;
    sigset_t mask_all, mask_one, prev_one;

    Sigfillset(&mask_all);  // 把所有信号加入集合
    Sigemptyset(&mask_one); // 空集合
    // 把SIGCHLD加入信号集
    Sigaddset(&mask_one, SIGCHLD);
    Signal(SIGCHLD, handler);
    initjobs(); /* Initialize the job list */

    while (1) {
        // 在fork前阻塞SIGCHLD信号
        Sigprocmask(SIG_BLOCK, &mask_one, &prev_one); /* Block SIGCHLD */
        if ((pid = Fork()) == 0) { /* Child process */
            // 子进程会继承父进程的阻塞集合，所以这里要恢复成prev_one
            Sigprocmask(SIG_SETMASK, &prev_one, NULL); /* Unblock SIGCHLD */
            Execve("/bin/date", argv, NULL);
        }
        // 访问全局数据结构（作业列表，为了安全要阻塞所有信号
        Sigprocmask(SIG_BLOCK, &mask_all, NULL); /* Parent process */  
        addjob(pid);  /* Add the child to the job list */
        // 添加作业后恢复到fork之前，现在可以接受SIGCHLD信号了
        Sigprocmask(SIG_SETMASK, &prev_one, NULL);  /* Unblock SIGCHLD */
    }
    exit(0);
}
/* $end procmask2 */
