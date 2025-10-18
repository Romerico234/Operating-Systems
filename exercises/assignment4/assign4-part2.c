#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_THREADS 100

void createPhilosophers(int nthreads);
void *philosopherThread(void *arg);
void thinking(int threadIndex);
void pickUpChopsticks(int threadIndex);
void eating(int threadIndex);
void putDownChopsticks(int threadIndex);

/*
 * N philosophers sitting at a round table
 * chopstick: binary semaphore per chopstick (0 = unavailable, 1 = available)
*/
static int N;
static sem_t chopstick[MAX_THREADS];

static int left_of(int i)  { 
    return i; 
}

static int right_of(int i) { 
    return (i + 1) % N; 
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N_Threads>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    if (N <= 1 || N > MAX_THREADS) {
        fprintf(stderr, "Error: N must be in [2, %d]\n", MAX_THREADS);
        return 1;
    }

    for (int i = 0; i < N; i++) {
        if (sem_init(&chopstick[i], 0, 1) != 0) {
            perror("sem_init");
            return 1;
        }
    }

    createPhilosophers(N);

    for (int i = 0; i < N; i++) {
        sem_destroy(&chopstick[i]);
    }

    return 0;
}

void createPhilosophers(int nthreads) {
    pthread_t tids[MAX_THREADS];
    int idx[MAX_THREADS];

    for (int i = 0; i < nthreads; i++) {
        idx[i] = i;
        pthread_create(&tids[i], NULL, philosopherThread, &idx[i]);
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join(tids[i], NULL);
    }
    
    printf("%d threads have been completed/joined successfully!\n", nthreads);
}

void *philosopherThread(void *arg) {
    int idx = *(int *)arg;

    thinking(idx);
    pickUpChopsticks(idx); 
    eating(idx);
    putDownChopsticks(idx);

    return NULL;
}

void thinking(int threadIndex) {
    useconds_t t = (useconds_t)((rand() % 500) + 1) * 1000;
    printf("Philosopher %d: thinking\n", threadIndex);
    usleep(t);
}

/* Asymmetric Solution + "only if both available"
 * - Even-numbered philosophers: pick up left first, then right
 * - Odd-numbered philosophers:  pick up right first, then left
 * - Use sem_trywait on both. If the second isn’t available, release the first immediately.
 * - No one holds one chopstick while waiting for the other (deadlock avoided).
 */
void pickUpChopsticks(int threadIndex) {
    int L = left_of(threadIndex);
    int R = right_of(threadIndex);

    while (1) {
        int first  = (threadIndex % 2 == 0) ? L : R;
        int second = (threadIndex % 2 == 0) ? R : L;

        if (sem_trywait(&chopstick[first]) == 0) {
            if (sem_trywait(&chopstick[second]) == 0) {
                return;
            }
            sem_post(&chopstick[first]);
        }

        usleep((useconds_t)((rand() % 3) + 1) * 1000);
    }
}

void eating(int threadIndex) {
    printf("Philosopher %d: starts eating\n", threadIndex);
    useconds_t t = (useconds_t)((rand() % 500) + 1) * 1000;  // 1–500 ms
    usleep(t);
    printf("Philosopher %d: ends eating\n", threadIndex);
}

void putDownChopsticks(int threadIndex) {
    int L = left_of(threadIndex);
    int R = right_of(threadIndex);
    sem_post(&chopstick[L]);
    sem_post(&chopstick[R]);
}