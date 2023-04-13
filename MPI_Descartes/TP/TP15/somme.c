#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "clock.h"

double SUM = 0.;
pthread_mutex_t LOCK;


struct thread_args {
    double *arr;
    int len;
};

typedef struct thread_args thread_args;

//Cette fonction est destinée à avoir en argument un (pointeur vers un) objet de type thread_args.
void* partial_sum_1(void *fun_args);

//Penser à forcer la liaison avec la bibliothèque mathématique à l'aide l'option -lm pour pouvoir utiliser sin.
double *create_array(int len)
{
    double *arr = malloc(len * sizeof(double));
    for (int i = 0; i < len; i++) 
    {
        arr[i] = sin(i);
    }
    return arr;
}


int main(int argc, char *argv[])
{
    return 0;
}
