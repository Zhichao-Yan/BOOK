/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-20 23:04:07
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-10-20 23:36:43
 * @FilePath: /book/Unix环境编程/代码/ch10/10-15.c
 * @Description: #
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */
#include "apue.h"
static void sig_quit(int);


int main(void)
{
    sigset_t oldset,newset,pendset;
    if(signal(SIGQUIT,sig_quit) == SIG_ERR)
        err_sys("can't catch SIGQUIT");
    sigemptyset(&newset);
    sigaddset(&newset,SIGQUIT);

    if(sigprocmask(SIG_BLOCK,&newset,&oldset) < 0)
    {
        err_sys("SIG_BLOCK error");
    }
    sleep(5);

    if(sigpending(&pendset) < 0)
    {
        err_sys("sigpending error");
    }
    if(sigismember(&pendset,SIGQUIT))
    {
        printf("\n SIGQUIT pending\n");
    }

    if(sigprocmask(SIG_SETMASK,&oldset,NULL) < 0)
        err_sys("sig_setmask error");
    printf("SIGQUIT unblocked\n");

    sleep(5);
    exit(0);

}

static void 
sig_quit(int signo)
{
    printf("Caught SIGQUIT\n");
    if(signal(SIGQUIT,SIG_DFL) == SIG_ERR)
        err_sys("cant't reset SIGQUIT");
}