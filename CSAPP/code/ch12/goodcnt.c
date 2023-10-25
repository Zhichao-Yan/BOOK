#include "csapp.h"

volatile long cnt = 0;
sem_t mutex;
void* thread(void* arg)
{
    long i;
    long niters = *((long*)arg);
    for(i = 0;i < niters;++i)
    {
        P(&mutex);
        cnt++;
        V(&mutex);
    }
    return NULL;
}
int main(int argc,char** argv)
{
    pthread_t t1,t2;
    long niters;
    if(argc != 2)
    {
        printf("usage: %s <niters>",argv[0]);
        exit(0);
    }
    niters = atoi(argv[1]);
    sem_init(&mutex,0,1);// sem_init deprecated 
    Pthread_create(&t1,NULL,thread,&niters);
    Pthread_create(&t2,NULL,thread,&niters);
    Pthread_join(t1,NULL);
    Pthread_join(t2,NULL);
    if(cnt != 2 * niters)
    {
        printf("Boom:cnt = %ld",cnt);
    }else
    {
        printf("Ok:cnt = %ld",cnt);
    }
    exit(0);
}