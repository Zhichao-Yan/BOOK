/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-21 12:42:16
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-10-21 19:04:02
 * @FilePath: /book/Unix环境编程/代码/ch12/12.8.1.c
 * @Description: 不同线程可以拥有不同的信号屏蔽集
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>


void* func(void *arg)
{
    sigset_t set2,oldset,mask;
    sigemptyset(&set2);
    sigaddset(&set2,SIGINT); // 把中断信号加入到信号集中
    sigaddset(&set2,SIGUSR1);
    if(pthread_sigmask(SIG_UNBLOCK,&set2,&oldset) != 0) // 从当前线程的信号集中取消屏蔽SIGINT
        printf("SIG_UNBLOCK error\n");


    if(pthread_sigmask(0,NULL,&mask) != 0) // 获取当前的屏蔽信号集
    {
        printf("error\n");
    }
    if(sigismember(&mask,SIGINT))
        printf("SIGINT当前被屏蔽在子线程\n");
    else
        printf("SIGINT当前没有被屏蔽在子线程\n");
    if(sigismember(&mask,SIGUSR1))
        printf("SIGUSR1当前被屏蔽在子线程\n");
    else
        printf("SIGUSR1当前没有被屏蔽在子线程\n");
    pthread_exit(NULL);
}
int main()
{
    sigset_t set1,oldset,mask;
    sigemptyset(&set1);
    sigaddset(&set1,SIGINT); // 把中断信号加入到信号集set1中
    sigaddset(&set1,SIGUSR1); // 把信号SIGUSR1加入到信号集set1中
    if(pthread_sigmask(SIG_BLOCK,&set1,&oldset) != 0) // 加到当前主线程的信号集中
        printf("SIG_BLOCK error\n");
    pthread_t id;
    pthread_create(&id,NULL,func,NULL);
    
    if(pthread_sigmask(0,NULL,&mask) != 0) // 获取当前的屏蔽信号集
    {
        printf("error\n");
    }
    sleep(4);
    if(sigismember(&mask,SIGINT))
        printf("SIGINT当前被屏蔽在主线程\n");
    else
        printf("SIGINT当前没有被屏蔽在主线程\n");
    if(sigismember(&mask,SIGUSR1))
        printf("SIGUSR1当前被屏蔽在主线程\n");
    else
        printf("SIGUSR1当前没有被屏蔽在主线程\n");

    if(pthread_sigmask(SIG_SETMASK,&oldset,NULL) != 0) // 从当前线程的信号集中取消屏蔽SIGINT
        printf("SIG_BLOCK error\n");
    pthread_join(id,NULL);
}