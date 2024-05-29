#include "csapp.h"

void *thread(void* arg)
{
    printf("Hello world! thread1\n");
    return NULL;
}

int main()
{
    pthread_t tid;
    Pthread_create(&tid,NULL,thread,NULL);
    printf("Hello world! thread2\n");
    Pthread_join(tid,NULL);
    exit(0);
}