/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-11-01 11:04:10
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-11-01 11:04:22
 * @FilePath: /book/CSAPP/code/ch7/int.c
 * @Description: 
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */
/* 
 * hello.c - Example program to demonstrate different ways to
 *           interpose on the malloc and free functions.
 *
 * Note: be sure to compile unoptimized (-O0) so that gcc won't
 * optimize away the calls to malloc and free.
 */
/* $begin interposemain */
#include <stdio.h>
#include <malloc.h>

int main()
{
    int *p = malloc(32);
    free(p);
    return(0); 
}
/* $end interposemain */