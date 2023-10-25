#include "semaphore.h"
int readcnt = 0;
sem_t mutex = 1;
sem_t cnt = 0;

void read(void)
{
    while(1)
    {
        sem_wait(&cnt);
        ++readcnt;
        if(readcnt == 1)
            sem_wait(&mutex);
        sem_post(&cnt);
        /* critical zone */
        /* read operation*/
        sem_wait(&cnt);
        --readcnt;
        if(readcnt == 0)
        {
            sem_post(&mutex);
        }
        sem_post(&cnt);
    }
}

void write(void)
{
    while(1)
    {
        sem_wait(&mutex);
        /* critical zone*/
        /* write operation*/
        sem_post(&mutex);
    }
}