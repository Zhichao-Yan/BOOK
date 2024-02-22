/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-29 20:45:06
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-10-29 21:31:19
 * @FilePath: /book/CSAPP/code/ch7/cal.c
 * @Description: 
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */

int num = 0;
int mult(int a,int b);
int add(int a,int b);

int main()
{
    int a = 4;
    int b = 5;
    int c = add(a,b);
    int d = mult(a,b);
    return 0;
}