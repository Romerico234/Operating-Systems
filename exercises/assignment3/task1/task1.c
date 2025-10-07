#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#ifdef _POSIX_THREADS
# include <pthread.h>
#endif
#include <semaphore.h>

void *text(void *arg);

int code[] = {4, 6, 3, 1, 5, 0, 2};

/* One semaphore per sentence (cases 0...6). We’ll open the first, keep the rest closed,
   so only the correct “next” thread can print. */
static sem_t S[7]; 

int main() {
    int i;
    pthread_t tid[7];
    
    /* Initialize the semaphore chain:
       - S[0] starts at 1  -> thread responsible for case 0 may run immediately.
       - S[1..6] start at 0 -> all others must wait until signaled by the previous case. */
    for (i = 0; i < 7; i++) {
        unsigned init = (i == 0) ? 1u : 0u;
        sem_init(&S[i], 0, init);
    }

    for (i = 0; i < 7; i++) {
        pthread_create(&tid[i], NULL, text, (void*)&code[i]);
    }
       
    for (i = 0; i < 7; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}

void *text(void *arg) {
    int n = *(int*)arg;

    /* Wait until it is THIS case’s turn. All threads call this,
       but only the one whose case matches the currently “open” semaphore proceeds. */
    sem_wait(&S[n]);

    switch (n) {
        case 0:
            printf("A semaphore S is an integer-valued variable which can take only non-negative values.\n");
            printf("Exactly two operations are defined on a semaphore:\n\n");
            break;
        case 1:
            printf("Signal(S): If there are processes that have been suspended on this semaphore,\n");
            printf("wake one of them, else S := S + 1.\n\n");
            break;
        case 2:
            printf("Wait(S): If S > 0 then S := S - 1, else suspend the execution of this process.\n");
            printf("The process is said to be suspended on the semaphore S.\n\n");
            break;
        case 3:
            printf("The semaphore has the following properties:\n\n");
            break;
        case 4:
            printf("1. Signal(S) and Wait(S) are atomic instructions.\n");
            printf("In particular, no instructions can be interleaved between the test that S > 0\n");
            printf("and the decrement of S or the suspension of the calling process.\n\n");
            break;
        case 5:
            printf("2. A semaphore must be given a non-negative initial value.\n\n");
            break;
        case 6:
            printf("3. The Signal(S) operation must wake one of the suspended processes.\n");
            printf("The definition does not specify which process will be awakened.\n\n");
            break;
    }

    /* Hand (signal) the turn to the NEXT case (unlock its semaphore), if any. */
    if (n < 6) {
        sem_post(&S[n + 1]);        
    }                               

    pthread_exit(0);
}
