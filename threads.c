#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

int has_common_factor(int number1, int number2);

void *phi_function(void *);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int out = 0;
int NTHREADS,inp;

int main(int argc, char *argv[])
{
    int c = 0;
    while(1){
        c = getopt(argc, argv, "p:n:");
        if (c == -1){
            break;
        }
        switch(c){
            case 'p': inp = atoi(optarg);
                break;
            case 'n': NTHREADS = atoi(optarg);
                break;
            default: printf("Invalid Option");
                return 1;
        }
    }
    //pthread_t thread_id[NTHREADS];
    pthread_t *thread_id;
    thread_id = (pthread_t *)malloc(sizeof(pthread_t) * NTHREADS);
    int i, j, arg;

    for (i = 0; i < NTHREADS; i++)
    {
        arg = i * inp / NTHREADS + 1;
        pthread_create(&thread_id[i], NULL, phi_function, (void *)&arg);
    }

    for (j = 0; j < NTHREADS; j++)
    {
        pthread_join(thread_id[j], NULL);
    }

    printf("Final value: %d\n", out);
}

void *phi_function(void *ptr)
{
    printf("threadid: %ld\n",pthread_self());
    int i;
    int *min;
    min = (int *)ptr;
    FILE *fptr;
    sleep(*min);
    char filename[100];
    sprintf(filename,"output%d", *min);

    fptr = fopen(filename,"w");

    for (i = *min; i < *min + inp / NTHREADS; i++)
    {
        int j;

        if (has_common_factor(i, inp) == 0) {
            pthread_mutex_lock(&mutex1);
            out++;
            printf("i = %d\n",i);
            pthread_mutex_unlock(&mutex1);
            fprintf(fptr,"i = %d\n",i);

        }
    }
    fclose(fptr);
}

int has_common_factor(int number1, int number2) {
    int j;
    for (j=2; j<=number1; j++) {
        if (number1 %j == 0 && number2 %j ==0) {
            return 1;
        }
    }
    return 0;
}