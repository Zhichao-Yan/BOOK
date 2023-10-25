#include "sbuf.h"
#include "csapp.h"

void sbuf_init(sbuf_t *b,int n)
{
    b->n  = n;
    b->buf = Calloc(n,sizeof(int));
    b->rear = b->front = 0;
    sem_init(&b->slot,0,n);
    sem_init(&b->item,0,0);
    sem_init(&b->mutex,0,1);
}
void sbuf_destroy(sbuf_t *b)
{
    Free(b->buf);
}


void sbuf_insert(sbuf_t *b,int item)
{
    P(&b->slot);
    P(&b->mutex);
    b->buf[(++b->rear)%(b->n)] = item;
    V(&b->mutex);
    V(&b->item);
}

int sbuf_remove(sbuf_t *b)
{
    P(&b->item);
    P(&b->mutex);
    int item = b->buf[(++b->front)%(b->n)];
    V(&b->mutex);
    V(&b->slot);
    return item;
}