#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

int has_common_factor(int number1, int number2);

void *phi_function(void *);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
volatile int out = 0;
int NTHREADS,inp;
volatile int counter=0;

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
    int i, j, interval;
    int *arg;
    interval = inp / NTHREADS;
    for (i = 0; i < NTHREADS; i++)
    {
	arg = (int *) malloc(sizeof(int) * 2);
        arg[0] = i * interval + 1;
	if (i==NTHREADS-1) {
		arg[1] = inp;
	} else {
		arg[1] = arg[0] + inp / NTHREADS;
	}
        pthread_create(&thread_id[i], NULL, phi_function, (void *)arg);
    }

    for (j = 0; j < NTHREADS; j++)
    {
        pthread_join(thread_id[j], NULL);
    }

    printf("Final value: %d\n", out);
}

void *phi_function(void *ptr)
{
    int i;
    int min, max;
    min = *(int *)ptr;
    max = *((int *)ptr + 1);
    for (i = min; i < max; i++)
    {

        if (has_common_factor(i, inp) == 0) {
            pthread_mutex_lock(&mutex1);
            out++;
            pthread_mutex_unlock(&mutex1);
        }
    pthread_mutex_lock(&mutex1);
    counter++;
    pthread_mutex_unlock(&mutex1);
   }
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
