# Part 1 Execution Instructions

```bash
cd assignment4/

gcc -Wall -Wextra -pthread assign4-part1.c -o assign4-part1

./assign4-part1 $N 
```

---

# Part 2 Execution Instructions

```bash
cd assignment4/

gcc -Wall -Wextra -pthread assign4-part2.c -o assign4-part2 -lrt

./assign4-part2 $N
```

> Note: POSIX unnamed semaphores (sem_init) work on Linux--macOS deprecated sem_init().
