# How to Run

This code only operates on Unix machines (uses POSIX). Note that, macOS has deprecated `sem_init/sem_destroy` so this code will not work on macOS.

```bash

cd task2/

gcc -Wall -pthread task2.c -o task2

./task2
```

# Explanation of Solution

This problem is similar to the bounded-buffer problem but with just a singular consumer (barber). 
- Use a mutex to protect the waiting room 
- Use a counting semaphore for waiting customers (similar to full)
- Use a counting semaphore for empty chairs (similar to empty)

