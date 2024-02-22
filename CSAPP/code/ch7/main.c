/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-25 19:15:15
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-11-01 11:03:52
 * @FilePath: /book/CSAPP/code/ch7/main.c
 * @Description: 
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */

/* main.c */
/* $begin main */
int sum(int *a, int n);

int array[2] = {1, 2};

int main() 
{
    int val = sum(array, 2);
    return val;
}
/* $end main */