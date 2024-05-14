#include "csapp.h"
#define N 4
void* thread(void* arg);

int main()
{
    pthread_t tid[N];
    for(int i = 0; i < N; ++i)
    {
        Pthread_create(&tid[i],NULL,thread,&i);
    }
    for(int i = 0; i < N; ++i)
    {
        Pthread_join(tid[i],NULL);
    }
    exit(0);
}
void* thread(void* arg)
{
    int id = *((int*)arg);
    printf("Hello World from thread%d\n",id);
    return NULL;
}