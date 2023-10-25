#include "pthread.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#define MAXTHREADS 32 
long len;
long nelem = 0;
long psum[MAXTHREADS];
void* accret(void* arg);
int main(int argc,char** argv)
{
    long i,result =0,nthread,myid[MAXTHREADS];
    pthread_t tid[MAXTHREADS];
    if (argc != 3){
        printf("Usage: %s <nthread> <log>\n", argv[0]); 
        exit (0);
    }
    nthread = atoi(argv[1]);// number of threads
    nelem = (1L << atoi(argv[2])); // number of elements waiting to be caluculated
    len = ceil(nelem/nthread);
    for(i = 0;i < nthread;++i)
    {
        myid[i] = i;
        pthread_create(&tid[i],NULL,accret,&myid[i]);
    }
    clock_t start = clock();
    for(i = 0;i < nthread;++i)
    {
        pthread_join(tid[i],NULL);
    }
    clock_t  stop = clock();
    for (i = 0; i < nthread; i++) // line:conc:psumarray:sumloop1
        result += psum[i];         // line:conc:psumarray:sumloop2
    printf("计算时间为：%f秒",(double)(stop-start)/CLOCKS_PER_SEC);
    if(result != (nelem * (nelem + 1)) / 2)
    {
        printf("Planed result= %ld\n",(nelem * (nelem + 1)) / 2);
        printf("Error result = %ld\n",result);
    }
    exit(0);
}
void* accret(void* arg)
{
    long id = *((long*)arg);
    long start = id * len + 1;
    long end = start + len - 1;
    long i;
    for(i  = start; i <= end&&i <= nelem; ++i)
    {
        psum[id] += i;
    }
    return NULL;
}