/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-21 19:08:31
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-10-21 20:48:08
 * @FilePath: /book/Unix环境编程/代码/ch12/12.8.2.c
 * @Description: 信号的处理是进程共享的
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sig1(int signo);
void sig2(int signo);
void sig3(int signo);

void* func(void *arg)
{
    printf("线程等待信号发生\n");
    sleep(5);
    pthread_exit(NULL);
}

int main()
{
    if(signal(SIGQUIT,sig1) == SIG_ERR)
        printf("signal error\n");
    printf("主线程等待信号发生\n");
    sleep(5);
    pthread_t id;
    while(1)
    {
        pthread_create(&id,NULL,func,NULL);
        pthread_join(id,NULL);
    }
    return 0;
}


void sig1(int signo)
{
    printf("信号处理函数是sig1\n");
    if(signal(SIGQUIT,sig2) == SIG_ERR)
        printf("signal error\n");
}
void sig2(int signo)
{
    printf("信号处理函数是sig2\n");
    if(signal(SIGQUIT,sig3) == SIG_ERR)
        printf("signal error\n");
}
void sig3(int signo)
{
    printf("信号处理函数是sig3\n");
    if(signal(SIGQUIT,sig1) == SIG_ERR)
        printf("signal error\n");
}