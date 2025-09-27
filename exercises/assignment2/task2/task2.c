#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5 // Number of threads to be created
int arr[NUM_THREADS] = {1, 2, 3, 4, 5}; // Array containing the numbers for which factorial will be calculated
int result = 0; // Global variable to store the final result (sum of squares of factorials)

typedef struct {
    int num; // Number for which factorial is to be calculated
    int factorial; // Factorial of the number
} ThreadParams;

void* factorion(void* arg) {
    ThreadParams *params = (ThreadParams*)arg;
    printf("Thread %lu calculating factorial of %d\n", (unsigned long)pthread_self(), params->num);
    for (int i = 1; i <= params->num; i++) { 
        params->factorial *= i;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t tids[NUM_THREADS];
    ThreadParams params[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        params[i].num = arr[i];
        params[i].factorial = 1;
        pthread_create(&tids[i], NULL, factorion, (void*)&params[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(tids[i], NULL);

        int factorial = params[i].factorial;

        printf("Factorial returned from this thread is %d\n", factorial);

        result += factorial;
    }

    printf("Sum of factorials: %d\n", result);
    return 0;
}