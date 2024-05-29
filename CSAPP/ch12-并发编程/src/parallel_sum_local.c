#include "pthread.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#define MAXTHREADS 32 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long sum = 0;
long len = 0;
long nelem = 0;
long psum[MAXTHREADS];
void* accret(void* arg)
{
    long id = *((long*)arg);
    long start = id * len + 1;
    long end = start + len - 1;
    long sum = 0;
    for(long i  = start; i <= end&&i <= nelem; ++i)
    {
        sum += i;
    }
    psum[id] = sum;
    return NULL;
}
int main(int argc,char** argv)
{
    if (argc != 3){
        printf("Usage: %s <nthread> <log>\n", argv[0]); 
        exit (0);
    }
    clock_t  start, stop;
    long nthread = atoi(argv[1]);// number of threads
    nelem = (1L << atoi(argv[2])); // number of elements waiting to be caluculated
    len = ceil(nelem/nthread);
    pthread_t tid[MAXTHREADS];
    long myid[MAXTHREADS];
    start = clock();
    for(long i = 0;i < nthread;++i)
    {
        myid[i] = i;
        pthread_create(&tid[i],NULL,accret,(void*)(&myid[i]));
    }
    for(long i = 0;i < nthread;++i)
    {
        pthread_join(tid[i],NULL);
    }
    long result = 0;
    for (long i = 0; i < nthread; i++) // line:conc:psumarray:sumloop1
        result += psum[i];         // line:conc:psumarray:sumloop2
    stop = clock();
    printf("计算时间为：%f秒",(double)(stop-start)/CLOCKS_PER_SEC);
    if(result != (nelem * (nelem + 1)) / 2)
    {
        printf("Planed result= %ld\n",(nelem * (nelem + 1)) / 2);
        printf("Error result = %ld\n",result);
    }
    exit(0);
}