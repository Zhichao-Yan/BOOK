#include "csapp.h"

void *thread(void* arg)
{
    printf("Hello world!!!");
    return NULL;
}
int main()
{
    pthread_t tid;
    Pthread_create(&tid,NULL,thread,NULL);
    Pthread_join(tid,NULL);
    exit(0);
}