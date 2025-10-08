#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_CUSTOMERS 100
#define MAX_CHAIRS    100

static sem_t mutex; // binary semaphore: protects the waiting-room buffer
static sem_t empty; // counting semaphore: number of free chairs (capacity N)
static sem_t full; // counting semaphore: number of waiting customers (0..M)

static int N, M;

// Circular buffer to carry customer IDs from producers -> consumer (FIFO) 
static int buf[MAX_CHAIRS];
static int in_idx = 0, out_idx = 0;

static int random_delay() { 
    return 1 + rand() % 5; 
}

// Barber (Consumer) Thread
void* barber(void* _) {
    while (1) {
        sem_wait(&full); // sleep until someone is waiting

        sem_wait(&mutex);
        int cid = buf[out_idx]; // take next customer ID (FIFO)
        out_idx = (out_idx + 1) % N;
        sem_post(&mutex);

        int secs = random_delay();
        printf("Barber starts cutting hair of Customer %d for %d s.\n", cid + 1, secs);
        sleep(secs);
        printf("Barber finishes cutting hair of Customer %d.\n", cid + 1);
        printf("Customer %d leaves after haircut.\n", cid + 1);

        sem_post(&empty); // one waiting chair becomes free
    }

    return NULL;
}

// Customer (Producer) Thread
void* customer(void* arg) {
    int id = *(int*)arg;

    printf("Customer %d arrives.\n", id + 1);

    // Try to claim a chair, if none, leave immediately
    if (sem_trywait(&empty) == 0) {
        sem_wait(&mutex);
        int chair_num = in_idx + 1; // assign readable chair number (1-based)
        buf[in_idx] = id;                  
        in_idx = (in_idx + 1) % N;
        printf("Customer %d sits in Chair %d in the waiting area.\n", id + 1, chair_num);
        sem_post(&mutex);

        sem_post(&full); // notify barber
    } else {
        printf("Customer %d leaves (no chairs).\n", id + 1);
    }

    return NULL;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <N_waiting_chairs> <M_customers>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);  
    M = atoi(argv[2]);
    if (N <= 0 || N > MAX_CHAIRS || M <= 0 || M > MAX_CUSTOMERS) {
        fprintf(stderr, "Invalid args: 0 < N <= %d, 0 < M <= %d\n", MAX_CHAIRS, MAX_CUSTOMERS);
        return 1;
    }

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, N);
    sem_init(&full,  0, 0);

    pthread_t barberThread;
    pthread_create(&barberThread, NULL, barber, NULL);

    pthread_t cthreads[MAX_CUSTOMERS];
    int ids[MAX_CUSTOMERS];

    // Create customers sequentially with random arrival times.
    for (int i = 0; i < M; i++) {
        sleep(random_delay());       
        ids[i] = i;                  
        pthread_create(&cthreads[i], NULL, customer, &ids[i]);
    }

    for (int i = 0; i < M; i++) {
        pthread_join(cthreads[i], NULL);
    }

    // When 'full' hits 0, the queue is empty; barber may still be cutting, so wait a bit.
    int f;
    do {
        sem_getvalue(&full, &f);
        sleep(5);
    } while (f > 0);

    printf("Barber goes to sleep.\n");

    // Stop the barber 
    pthread_cancel(barberThread);
    pthread_join(barberThread, NULL);

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}