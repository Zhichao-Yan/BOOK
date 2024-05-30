#include "csapp.h"

static int byte_cnt;
static sem_t mutex; //改有名信号量

static void init_echo_cnt(void)
{
    sem_init(&mutex,0,1); //初始化mutex
    byte_cnt = 0;   // 初始化计数器
}

void echo_cnt(int connfd)
{
    int n;
    char buf[MAXLINE];
    rio_t rio;
    /* once_control总是被初始化为PTHREAD_ONCE_INIT */
    static pthread_once_t once_control = PTHREAD_ONCE_INIT;
    /*     
    动态初始化多个线程共享的全局变量/或静态全局,这里是byte_cnt: 各个线程收到的字节总数
    任意线程第一次使用onc_control调用pthread_once时，它调用init_echo_cnt函数初始化初始化全局变量
    任意线程第二次调用pthread_once时，啥也不改变 
    每一次echo_cnt的调用，都会调用pthread_once，但是它大多数时候不做任何事情
    */
    Pthread_once(&once_control,init_echo_cnt);
    Rio_readinitb(&rio,connfd);     // 初始化RIO缓冲
    // 从缓冲中读取数据到buf
    while((n = Rio_readlineb(&rio,buf,MAXLINE)) != 0)
    {
        P(&mutex);
        byte_cnt += n;
        printf("server received %d(total %d) bytes on fd-%d\n",n,byte_cnt,connfd);
        V(&mutex);
        Rio_writen(connfd,buf,n); // 从buf写回到connfd中
    }
}