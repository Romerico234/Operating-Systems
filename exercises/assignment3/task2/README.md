# How to Run

This code only operates on Unix machines (uses POSIX). Note that, macOS has deprecated `sem_init/sem_destroy` so this code will not work on macOS.

```bash

cd task2/

gcc -Wall -pthread task2.c -o task2

./task2 $N $M
```

# Explanation of Solution

The solution I proposed is similar to the solution to the bounded-buffer problem. The barber acts as our singular consumer, waiting on the `full` semaphore until a customer is ready, while each customer acts as a producer, signaling the barber when they take a seat. The `mutex` semaphore prevents race conditions when multiple customers attempt to sit simultaneously. The `empty` and `full` semaphores track the number of available chairs and waiting customers. Customers arrive at random intervals and if no chairs are available, they leave immediately. Otherwise, they take a chair and wait for the barber. The barber continuously serves customers in FIFO order, cutting hair for a random duration before freeing a chair for the next waiting customer. 