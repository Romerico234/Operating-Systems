#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 100

static const char *NAME = "Romerico David";

void createPhilosophers(int nthreads);
void *philosopherThread(void *pVoid);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N_Threads>\n", argv[0]);
        return 1;
    }

    int nthreads = atoi(argv[1]);
    if (nthreads <= 0 || nthreads > MAX_THREADS) {
        fprintf(stderr, "Error: N must be in [1, %d]\n", MAX_THREADS);
        return 1;
    }

    printf("%s: Assignment 4: # of threads = %d\n", NAME, nthreads);

    createPhilosophers(nthreads);

    return 0;
}

void createPhilosophers(int nthreads) {
    pthread_t tids[MAX_THREADS];
    int philosopherIdx[MAX_THREADS];

    for (int i = 0; i < nthreads; i++) {
        philosopherIdx[i] = i;
        pthread_create(&tids[i], NULL, philosopherThread, &philosopherIdx[i]);
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join(tids[i], NULL);
    }

    printf("%d threads have been completed/joined successfully!\n", nthreads);
}

void *philosopherThread(void *pVoid) {
    int idx = *(int *)pVoid;
    printf("This is philosopher %d\n", idx);
    return NULL; 
}
