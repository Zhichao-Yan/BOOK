#include "semaphore.h"

typedef struct {
    int n;
    int *buf;
    int front;
    int rear;
    sem_t slot;
    sem_t item;
    sem_t mutex;
}sbuf_t;