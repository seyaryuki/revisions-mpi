#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t mutex;
sem_t empty, full;

void* produce(void* arg)
{
    for (int i=0; i<20; i++)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        printf("Production %d\n", i);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

void* consume(void* arg);

int main(void)
{
    return 0;
}