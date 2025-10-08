# How to Run

This code only operates on Unix machines (uses POSIX). Note that, macOS has deprecated `sem_init/sem_destroy` so this code will not work on macOS.

```bash
cd task1/

gcc -Wall -pthread task1.c -o task1

./task1
```

# Explanation of Solution
I created a static global array `S` of size 7. Each element represents one semaphore per case (from cases 0...6). We’ll unlock the first semaphore but keep the rest locked so that only the correct “next” thread can print. 

```c
void *text(void *arg);

int code[] = {4, 6, 3, 1, 5, 0, 2};

static sem_t S[7]; 
```

Next, I added a for loop to initalize my semaphores. Again:
    - `S[0]` starts at 1 as its the thread responsible for case 0 which is the first case that we want to run 
    - S[1..6] start at 0 and must wait until signaled exactly by the previous case
```c
int main() {
    int i;
    pthread_t tid[7];

    for (i = 0; i < 7; i++) {
        unsigned init = (i == 0) ? 1u : 0u;
        sem_init(&S[i], 0, init);
    }

    ...
}
```

Lastly, in the thread function, I ensured that the thread handling whichever case is waiting until the currently open sempahore matches it. This guarantees that, despite the random order in `code[]`, the cases execute from 0 to 6. After printing, the thread signals the next case by posting (unlocking) its semaphore.
```c
void *text(void *arg) {
    int n = *(int*)arg;

    sem_wait(&S[n]);

    switch (n) {

        ...

    }

    if (n < 6) {
        sem_post(&S[n + 1]);        
    }                               

    pthread_exit(0);
}

```