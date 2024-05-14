/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-25 19:16:35
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-10-25 19:17:29
 * @FilePath: /book/CSAPP/code/ch7/multvec.c
 * @Description: 
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */
/* multvec.c */
/* $begin multvec */
int multcnt = 0;

void multvec(int *x, int *y, 
	     int *z, int n) 
{
    int i;

    multcnt++;

    for (i = 0; i < n; i++)
	z[i] = x[i] * y[i];
}
/* $end multvec */
