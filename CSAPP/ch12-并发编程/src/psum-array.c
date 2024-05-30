#include "csapp.h"
#define MAXTHREADS 32

/* 
执行参数1：./bin/psum-array 1 31
计算时间1：4.814300秒

执行参数2: ./bin/psum-array 2 31
计算时间2: 5.443870秒

执行参数3: /bin/psum-array 4 31
计算时间3: 9.425474秒

执行参数4: ./bin/psum-array 8 31
计算时间4: 7.968204秒

执行参数5: ./bin/psum-array 16 31
计算时间5: 6.987187秒
*/


void *sum_array(void *vargp); 
long psum[MAXTHREADS];  
long nelems_per_thread; 

int main(int argc, char **argv)
{
    long i, nelems, log_nelems, nthreads, myid[MAXTHREADS], result = 0;
    pthread_t tid[MAXTHREADS];

    if (argc != 3)
    {
        printf("Usage: %s <nthreads> <log_nelems>\n", argv[0]);
        exit(0);
    }
    nthreads = atoi(argv[1]);
    log_nelems = atoi(argv[2]);
    nelems = (1L << log_nelems);

    if ((nelems % nthreads) != 0 || (log_nelems > 31))
    {
        printf("Error: invalid nelems\n");
        exit(0);
    }
    nelems_per_thread = nelems / nthreads;

    clock_t start = clock();
    for (i = 0; i < nthreads; i++)
    {                                                       
        myid[i] = i;                                        
        Pthread_create(&tid[i], NULL, sum_array, &myid[i]); 
    }                                                       
    for (i = 0; i < nthreads; i++)                          
        Pthread_join(tid[i], NULL);                         

    for (i = 0; i < nthreads; i++) 
        result += psum[i];         
    clock_t stop = clock();

    printf("计算时间为：%f秒\n", (double)(stop - start) / CLOCKS_PER_SEC);
    if (result != (nelems * (nelems - 1)) / 2) 
        printf("Error: result=%ld\n", result); 

    exit(0);
}


void *sum_array(void *vargp)
{
    long myid = *((long *)vargp); 
    long start = myid * nelems_per_thread; 
    long end = start + nelems_per_thread; 
    long i;

    for (i = start; i < end; i++)
    {                    
        psum[myid] += i; 
    }                    
    return NULL;
}