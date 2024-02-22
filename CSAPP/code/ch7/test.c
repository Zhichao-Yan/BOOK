/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-27 20:06:14
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-10-28 10:29:44
 * @FilePath: /book/CSAPP/code/ch7/test.c
 * @Description: 
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */
void print(){
     static int flag_local_static = 100;
     flag_local_static++;
}
extern int sum(int *a, int n);
static int max(int a, int b)
{
    if(a > b)
        return a;
    return b;
}

int flag_uninitialized;
int flag_0initialized = 0;
int flag_1initialized = 100;
int array[2] = {10,10};
static int flag_global_static_uninitialized;
static int flag_global_static_0initialized = 0;
static int flag_global_static_1initialized = 1;



int main()
{
    int val = sum(array, 2);
    int bigger = max(val,10);
    return val;
}