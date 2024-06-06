/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-21 20:34:04
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-10-21 20:41:09
 * @FilePath: /book/Unix环境编程/代码/ch12/12.8.3.c
 * @Description: sigwait函数测试
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int signo;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGINT);
    sigaddset(&mask,SIGQUIT);
    while(1)
    {
        sigwait(&mask,&signo);
        if(signo == SIGINT)
            printf("received SIGINT\n");
        else if(signo == SIGQUIT)
            printf("received SIGQUIT\n");
        else    
            break;
    }
    return 0;
}