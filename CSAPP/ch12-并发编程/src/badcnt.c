#include "csapp.h"

volatile long cnt = 0;
void* thread(void* arg)
{
    long i;
    long niters = *((long*)arg);
    for(i = 0;i < niters;++i)
    {
        ++cnt;
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
    Pthread_create(&t1,NULL,thread,&niters);
    Pthread_create(&t2,NULL,thread,&niters);
    Pthread_join(t1,NULL);
    Pthread_join(t2,NULL);
    if(cnt != 2 * niters)
    {
        printf("Boom:cnt = %ld\n",cnt);
    }else
    {
        printf("Ok:cnt = %ld\n",cnt);
    }
    exit(0);
}