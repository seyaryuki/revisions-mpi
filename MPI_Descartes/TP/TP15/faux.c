#include <stdio.h>
#include <pthread.h>

//Commande préprocesseur permettant de remplacer toutes les occurrences de NB_THREADS par 2.
//Evite d'écrire 2 partout à la place de NB_THREADS, et permet de modifier facilement cette valeur.
#define NB_THREADS 2

void* f(void* arg)
{
    int index = *(int*)arg;
    for (int i = 0; i < 10; i++) 
    {
        printf("Thread %d : %d\n", index, i);
    }
    return NULL;
}

int main(void)
{
    //NB_THREADS est connu à la compilation donc on peut définir un tableau statique comme suit :
    pthread_t threads[NB_THREADS];
    printf("Before creating the threads.\n");
    for (int i = 0; i < NB_THREADS; i++) 
    {
        int thread_index = i;
        pthread_create(&threads[i], NULL, f, &thread_index);
    }
    printf("While the other threads are running.\n");
    for (int i = 0; i < NB_THREADS; i++) 
    {
        pthread_join(threads[i], NULL);
    }
    printf("After the other threads have stopped running.\n");
    return 0;
}

