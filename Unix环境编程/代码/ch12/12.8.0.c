/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-21 10:53:25
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-10-22 09:34:19
 * @FilePath: /book/Unix环境编程/代码/ch12/12.8.0.c
 * @Description: 测试pthread_sigmask
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */
#include "apue.h"
#include <pthread.h>

int quitflag;
sigset_t mask;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

void* thr_fn(void *arg)
{
    int err,signo;
    for(; ;)
    {
        err = sigwait(&mask,&signo);
        if(err != 0)
        {
            err_exit(err,"sigwait failed");
        }
        switch(signo)
        {
            case SIGINT:
                printf("\ninterrupt\n");
                break;
            case SIGQUIT:
                pthread_mutex_lock(&lock);
                quitflag = 1;
                pthread_mutex_unlock(&lock);
                pthread_cond_signal(&waitloc);
                return (0);
            default:
                printf("unexpected signal %d",signo);
                exit(1);
        }
    }
}
int main(void)
{
    int err;
    sigset_t oldmask;
    pthread_t tid;

    sigemptyset(&mask);
    sigaddset(&mask,SIGQUIT);
    sigaddset(&mask,SIGINT);
    if((err = pthread_sigmask(SIG_BLOCK,&mask,&oldmask)) != 0)
    {
        err_exit(err,"SIG_BLOCK error");
    }
    err = pthread_create(&tid,NULL,thr_fn,0);
    if(err != 0)
        err_exit(err,"can't creat thread");
    
    pthread_mutex_lock(&lock);
    while(quitflag == 0)
        pthread_cond_wait(&waitloc,&lock);
    pthread_mutex_unlock(&lock);

    quitflag = 0;

    if(sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
        err_sys("SIG_SETBLOCK error");
    exit(0);
}