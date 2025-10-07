# How to Run

```bash
gcc -Wall -pthread task1.c -o task1

./task1
```

# Explanation of Solution

Follow up questions:

1. Why is pshared set to 0 in sem_init here, and what would change if it were 1?
2. Suppose we accidentally initialized all seven semaphores to 1. What behavior would you expect and why?
3. What’s the downside of using a single global semaphore for all cases instead of a chain (hint: how would you wake the right next thread)?
4. Extra thought: could you solve the same ordering with a mutex + condition variable (pthread_cond_t) instead of semaphores? What would the shared state be?