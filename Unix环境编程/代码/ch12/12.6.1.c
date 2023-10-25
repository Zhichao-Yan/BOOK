/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-22 09:34:48
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-10-22 11:01:59
 * @FilePath: /book/Unix环境编程/代码/ch12/12.6.1.c
 * @Description: 线程特有数据
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#define threads 10
pthread_key_t key;

pthread_once_t flag = PTHREAD_ONCE_INIT;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct info{
    char thread_name[10];
    char thread_time[20];
}info;

info th[threads];
void print_info(void);
void creat_key(void);
void* func(void *arg);

void print_info(void)
{
    int *id = pthread_getspecific(key);
    pthread_mutex_lock(&mutex);
    printf("名字:%s\n",th[*id].thread_name);
    printf("时间:%s\n",th[*id].thread_time);
    pthread_mutex_unlock(&mutex);
}

void creat_key(void)
{
    if( pthread_key_create(&key,free) != 0)
    {
        printf("pthread_key_create error\n");
    }
}
void* func(void *arg)
{
    int i = (int)arg;
    if(pthread_once(&flag,creat_key) != 0)
        printf("pthread_once error\n");
    int *id = (int*) malloc(sizeof(int));
    if(id == NULL)
        printf("malloc failed\n");
    *id = i;
    if(pthread_setspecific(key,id) != 0)
        printf("error!");
    print_info();
    pthread_exit(NULL);
}
int main()
{
    pthread_t id[threads];
    for(int i = 0; i < threads; ++i)
    {
        time_t rawtime;
        time(&rawtime);
        sprintf(th[i].thread_time,"%s",ctime(&rawtime));
        sprintf(th[i].thread_name,"线程%d",i+1);
        pthread_create(&id[i],NULL,func,(void*)(i + 0));
        sleep(1);
    }
    for(int i = 0; i < threads; ++i)
    {
        pthread_join(id[i],NULL);
    }
    return 0;
}