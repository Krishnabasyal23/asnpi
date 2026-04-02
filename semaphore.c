#include "semaphore.h"

void sem_init_custom(semaphore_t* s,int val) {
    s->count = val;
    pthread_mutex_init(&s->mutex,NULL);
    pthread_cond_init(&s->cond,NULL);
}

void sem_wait_custom(semaphore_t* s) {
    pthread_mutex_lock(&s->mutex);
    while(s->count<=0) pthread_cond_wait(&s->cond,&s->mutex);
    s->count--;
    pthread_mutex_unlock(&s->mutex);
}

void sem_post_custom(semaphore_t* s) { 
    pthread_mutex_lock(&s->mutex);
    s->count++;
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock(&s->mutex);
}