#include "csapp.h"
#include "time.h"
#define MAXTHREADS 32

/* 
执行参数1：./bin/psum-mutex 1 31
计算时间1：19.772023秒
执行参数2: ./bin/psum-mutex 2 31
计算时间2: 139.022782秒

执行参数3: ./bin/psum-mutex 4 31
计算时间3: 265.566905秒

执行参数4: ./bin/psum-mutex 8 31
计算时间4: 318.558826秒

执行参数5: ./bin/psum-mutex 16 31
计算时间5: 574.655059秒
*/


void *sum_mutex(void *vargp); // 线程例程

long gsum = 0;              // 全局和
long nelems_per_thread;     // 单个线程计算数量
sem_t mutex;                // 互斥锁

int main(int argc, char **argv)
{
    long i, nelems, log_nelems, nthreads, myid[MAXTHREADS];
    pthread_t tid[MAXTHREADS];

    if (argc != 3)
    {
        printf("Usage: %s <nthreads> <log_nelems>\n", argv[0]);
        exit(0);
    }
    nthreads = atoi(argv[1]);
    log_nelems = atoi(argv[2]);
    nelems = (1L << log_nelems);

    /* 参数是否符合要求 */
    if ((nelems % nthreads) != 0 || (log_nelems > 31))
    {
        printf("Error: invalid nelems\n");
        exit(0);
    }
    nelems_per_thread = nelems / nthreads;  // 当个线程计算元素个数
    sem_init(&mutex, 0, 1);
    /* 计算时间 */
    clock_t start = clock();
    /* 创建线程*/
    for (i = 0; i < nthreads; i++)
    {                                                       
        myid[i] = i;                                        
        Pthread_create(&tid[i], NULL, sum_mutex, &myid[i]); 
    }                   
    /* 等待线程结束 */                                    
    for (i = 0; i < nthreads; i++)                          
        Pthread_join(tid[i], NULL);                         
    clock_t stop = clock();
    /* 统计耗时 */
    printf("计算时间为：%f秒\n", (double)(stop - start) / CLOCKS_PER_SEC);  
    /* 检查计算结果 */
    if (gsum != (nelems * (nelems - 1)) / 2) 
        printf("Error: result=%ld\n", gsum); 

    exit(0);
}


void *sum_mutex(void *vargp)
{
    long myid = *((long *)vargp);   // 线程id        
    long start = myid * nelems_per_thread; // 起始计算点
    long end = start + nelems_per_thread;  // 结尾计算点 
    long i;

    for (i = start; i < end; i++)
    {                                 
        P(&mutex);   
        gsum += i;                    
        V(&mutex); 
    }                                 
    return NULL;
}