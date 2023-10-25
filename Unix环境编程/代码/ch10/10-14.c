/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-20 21:13:42
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-10-20 21:25:00
 * @FilePath: /book/Unix环境编程/代码/ch10/10-14.c
 * @Description: 
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */
#include "apue.h"
#include <errno.h>

void pr_mask(const char* str)
{
    sigset_t sigset;
    int errno_save;
    errno_save = errno;
    if(sigprocmask(0,NULL,&sigset) < 0)
    {
        err_ret("sigprocmask error");
    }else{
        printf("%s",str);
        if(sigismember(&sigset,SIGINT))
            printf(" SIGINT");
        if(sigismember(&sigset,SIGQUIT))
            printf(" SIGQUIT");
        if(sigismember(&sigset,SIGUSR1))
            printf(" SIGUSR1");
        if(sigismember(&sigset,SIGALRM))
            printf(" SIGALRM");
        printf("\n");
    }
    errno = errno_save;
}