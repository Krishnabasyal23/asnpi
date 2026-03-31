#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <pthread.h>

typedef struct {
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} semaphore_t;

void sem_init_custom(semaphore_t* s,int val);
void sem_wait_custom(semaphore_t* s);
void sem_post_custom(semaphore_t* s);

#endif