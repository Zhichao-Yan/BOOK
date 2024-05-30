#include "sbuf.h"
#include "csapp.h"

/* 初始化sbuf */
void sbuf_init(sbuf_t *b,int n)
{
    b->n  = n;  // 缓冲区大小为n
    b->buf = Calloc(n,sizeof(int)); // 动态分配内存，并置0
    b->rear = b->front = 0;     // 初始化头和尾的位置
    sem_init(&b->slot,0,n);     // 初始化空槽信号量
    sem_init(&b->item,0,0);     // 初始化项目信号量
    sem_init(&b->mutex,0,1);    // 初始化互斥信号量
}
/* 消耗sbuf */
void sbuf_deinit(sbuf_t *b)
{
    Free(b->buf);
}

/* 插入项目 */
void sbuf_insert(sbuf_t *b,int item)
{
    P(&b->slot);    // 等待一个有一个空槽位
    P(&b->mutex);   // 给缓冲加锁，获得访问权
    b->buf[(++b->rear)%(b->n)] = item; // 添加项目
    V(&b->mutex);   // 给缓冲解锁，释放访问权
    V(&b->item);    // 宣布有新项目可用
}

/* 移除项目 */
int sbuf_remove(sbuf_t *b)
{
    P(&b->item);    // 等待有可用的项目
    P(&b->mutex);   // 加锁获得缓冲区访问权
    int item = b->buf[(++b->front)%(b->n)]; // 取走项目
    V(&b->mutex);   // 解锁释放缓冲区访问权
    V(&b->slot);    // 宣布有新的空槽使用
    return item;    // 返回项目
}