#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static sem_t mutex; // binary semaphore to protect the waiting room "buffer"
static sem_t empty; // counting semaphore for how many empty chairs remain (capacity N)
static sem_t full;  // counting semaphore for how many customers are waiting

static int N, M;

// Barber (Consumer) Thread Function 
void* barber(void* _) {
    while (1) {
        sem_wait(&full);     
        sem_wait(&mutex);   
        sem_post(&mutex);

        int secs = 1 + rand() % 5;
        printf("Barber: cutting hair for %d s...\n", secs);
        sleep(secs);
        printf("Barber: finished a haircut.\n");

        sem_post(&empty);
    }
    return NULL;
}

// Customer (Producer)
void* customer(void* arg) {
    int id = *(int*)arg;

    sleep(1 + rand() % 5);
    printf("Customer %d arrives.\n", id);

    if (sem_trywait(&empty) == 0) {
        sem_wait(&mutex);
        printf("Customer %d sits in the waiting area.\n", id);
        sem_post(&mutex);

        sem_post(&full);
    } else {
        printf("Customer %d leaves (no chairs).\n", id);
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <N_waiting_chairs> <M_customers>\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);  M = atoi(argv[2]);
    if (N <= 0 || M < 0) { fprintf(stderr, "Bad args.\n"); return 1; }

    srand((unsigned)time(NULL));

    sem_init(&mutex, 0, 1);   // unlocked
    sem_init(&empty, 0, N);   // N chairs available
    sem_init(&full,  0, 0);   // 0 waiting customers

    pthread_t barb;
    pthread_create(&barb, NULL, barber, NULL);

    pthread_t* cthr = malloc(sizeof(pthread_t) * (size_t)M);
    int* ids = malloc(sizeof(int) * (size_t)M);

    for (int i = 0; i < M; i++) {
        ids[i] = i + 1;
        pthread_create(&cthr[i], NULL, customer, &ids[i]);
    }

    for (int i = 0; i < M; i++) pthread_join(cthr[i], NULL);

    pthread_cancel(barb);
    pthread_join(barb, NULL);

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    free(cthr);
    free(ids);
    return 0;
}
