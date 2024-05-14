/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-25 19:22:32
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-10-25 19:22:36
 * @FilePath: /book/CSAPP/code/ch7/main2.c
 * @Description: 
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */
/* main2.c */
/* $begin main2 */
#include <stdio.h>

void addvec(int *x, int *y,int *z, int n);
void multvec(int *x, int *y,int *z, int n); 

int x[2] = {1, 2};
int y[2] = {3, 4};
int z[2];

int main() 
{
    addvec(x, y, z, 2);
    printf("z = [%d %d]\n", z[0], z[1]);
    return 0;
}
/* $end main2 */
