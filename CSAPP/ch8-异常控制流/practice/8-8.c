#include "csapp.h"
#include <signal.h>

/* $begin signalprob0 */
// 对全局变量，使用volatile，更为安全，表明这是一个经常变化的变量
// 避免编译器优化，每一次都从内存中读取，而不是寄存器。
volatile long counter = 2;


// 在访问全局变量时，阻塞所有信号
void handler1(int sig) 
{
    sigset_t mask, prev_mask;

    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask);  /* Block sigs */
    Sio_putl(--counter);
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL); /* Restore sigs */

    _exit(0);
}

int main() 
{
    pid_t pid;
    sigset_t mask, prev_mask;

    printf("%ld", counter);
    fflush(stdout);

    signal(SIGUSR1, handler1);
    if ((pid = Fork()) == 0) {
        while(1) {};    // 在子进程中收到信号SIGUSR1
    }
    Kill(pid, SIGUSR1); // kill函数传递信号给子进程
    Waitpid(-1, NULL, 0);

    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask);  /* Block sigs */
    printf("%ld", ++counter);
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL); /* Restore sigs */

    exit(0);
}
/* $end signalprob0 */

// 打印213
// 注意是父进程和子进程
// 父进程发送信号给子进程，2个全局变量是独立的