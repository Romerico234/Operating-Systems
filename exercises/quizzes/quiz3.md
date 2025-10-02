# What is critical section and busy waiting? Provide an explanation of the three criteria used to solve the critical section problem. 


**Critical section.** The code region where a process/thread accesses shared data; at most one process may execute its critical section at a time.

**Busy waiting.** Spinning in a loop (e.g. `while (lock) ;`) until a condition becomes true; wastes CPU cycles but avoids blocking (appears in spinlocks, `test_and_set`, naive semaphores).

**Three criteria (and why they matter):**
1. **Mutual Exclusion.** If some \(P_i\) is in its CS, no other \(P_j\neq i\) can be in its CS. *Prevents race conditions → correctness of shared data.*
2. **Progress.** If no one is in a CS and some want to enter, the choice of who enters next cannot be postponed indefinitely (depends only on contenders). *Prevents indefinite postponement by irrelevant processes.*
3. **Bounded Waiting.** After a process requests entry, there is a bound on how many times others can enter before it does. *Prevents starvation under contention.*

---

# Discuss deadlock and starvation. List the four necessary conditions characterizing deadlock and elucidate how these conditions contribute to deadlock. 


A **deadlock** occurs when **two or more processes are waiting indefinitely** for an event that can only be triggered by one of the waiting processes. In this situation, none of the processes can proceed, creating a circular wait.

**Starvation** (also called **indefinite blocking**) is not the same as deadlock. It occurs when a process **never gets scheduled access to resources** because higher-priority processes keep preempting it. The process is ready to execute but is indefinitely postponed.  

**Four necessary (Coffman) conditions** (all must hold simultaneously):
1. **Mutual Exclusion:** Only one process at a time can use a resource.
2. **Hold and Wait:** A process is holding at least one resource and is waiting to acquire additional resources held by others.
3. **No Preemption:** A resource cannot be forcibly taken away. It can only be released voluntarily by the process holding it, after completing its task.
4. **Circular Wait:** A directed cycle of waits among processes exists

**Why they cause deadlock:** Non-sharable + holding while requesting + no preemption ⇒ waits can’t be broken; with a wait cycle, no one in the cycle can proceed.

---

# Question 1: Peterson’s Solution (5 points)

## (a) Explain Peterson’s solution for the two-process critical section problem. Use pseudocode/some form of code to illustrate how it works.

**Shared State:**
```c
// shared
int turn; // whose turn it is
bool flag[2] = {false, false}; // flag[i] = true iff Pi wants to enter
```

**Process $P_i$:**
```c
do {
    flag[i] = true; // I want to enter
    turn = j; // let the other have priority
    while (flag[j] && turn == j) ;  // busy wait

    /* --- critical section --- */

    flag[i] = false;     // I'm leaving

    /* --- remainder section --- */
} while (true);
```

## (b) Show how Peterson’s solution satisfies the three requirements of the Critical Section Problem: (a) Mutual Exclusion, (b) Progress, (c) Bounded Waiting 

- **Mutual Exclusion:**  $P_i$ enters CS only if `flag[j] = false` **OR** `turn = i` ⇒ Both cannot be in CS at the same time

- **Progress:** If no one is in CS:
  - If only one wants in, its `while` guard is false (either `flag[j] == false` or `turn != j` eventually), so it enters. If both contend, the **last write to `turn`** grants priority to the other; exactly one proceeds.

- **Bounded Waiting:** When \(P_i\) leaves, it sets `flag[i] = false`. If both keep requesting, priority alternates via `turn`, ensuring each waiting process enters after at most one entry by the other.


## (c) Why Peterson’s solution is not suitable for modern computers. 

- **Scales poorly:** Two-process only; busy-waits waste CPU cycles; cache-coherence traffic is heavy.

---

# Question 2: Semaphores and the Dining Philosophers 

## (a) Define a semaphore (1 point). Identify the problem that occurs when all philosophers attempt to pick up their left chopstick simultaneously. 


- **Semaphore:** Shared, non-negative integer used to control the CS and manipulated only by atomic operations `wait(P)` / `signal(V)`; controls access to critical sections/resources. An atomic operations ensures it cannot be interrupted during execution and that only one process at a time can modify the lock variable.

- **Problem when all pick left simultaneously:** Everyone holds one chopstick and waits for the other which causes a **deadlock**.


## (b) Propose two semaphore-based solutions (excluding Monitors) to resolve this issue.

Each philosopher `i` follows this loop:

```c
do {
    wait(chopstick[i]);              // pick up left chopstick
    wait(chopstick[(i+1) % 5]);      // pick up right chopstick

    // ---- Eat ----

    signal(chopstick[i]);            // put down left chopstick
    signal(chopstick[(i+1) % 5]);    // put down right chopstick

    // ---- Think ----
} while (true);
```

The **first solution** is to limit the number of philosophers allowed to sit at the table:
```c
semaphore room = 4;                     // at most 4 philosophers enter
semaphore chopstick[5] = {1,1,1,1,1};

philosopher i:
do {
    wait(room);                          // enter dining room
    wait(chopstick[i]);                  // pick up left
    wait(chopstick[(i+1)%5]);            // pick up right

    // ---- Eat ----

    signal(chopstick[(i+1)%5]);          // put down right
    signal(chopstick[i]);                // put down left
    signal(room);                        // leave dining room

    // ---- Think ----
} while (true);
```

The **second solution** checks both are available before picking (semaphores, no busy waiting).
```c
// Shared
semaphore mutex = 1; // protects state
semaphore self[5] = {0,0,0,0,0};  // per-philosopher gates (start at 0 => block)
enum { THINKING, HUNGRY, EATING } state[5];

int LEFT(int i)  { return (i + 4) % 5; }
int RIGHT(int i) { return (i + 1) % 5; }

void test(int i) {
    // Can i eat? Only if neighbors aren't eating.
    if (state[i] == HUNGRY &&
        state[LEFT(i)]  != EATING &&
        state[RIGHT(i)] != EATING) {
        state[i] = EATING;
        signal(self[i]); // wake philosopher i (both chopsticks available)
    }
}

void pickup(int i) {
    wait(mutex);
    state[i] = HUNGRY;
    test(i); // check both before picking
    signal(mutex);
    wait(self[i]); // block if not granted (no busy waiting)
}

void putdown(int i) {
    wait(mutex);
    state[i] = THINKING;
    // releasing chopsticks may enable neighbors
    test(LEFT(i));
    test(RIGHT(i));
    signal(mutex);
}

// Philosopher i loop:
do {
    pickup(i);

    // ---- Eat ----

    putdown(i);

    // ---- Think ----
} while (true);
```

A **third solution** is making use of asymmetric order (odd/even reversal to break the cycle).
   - Odd-numbered philosophers pick up left chopstick first, then right
   - Even-numbered philosophers pick up right chopstick first, then left  
   - Breaks circular wait, avoiding deadlock

```c
semaphore chopstick[5] = {1,1,1,1,1};

philosopher i:
do {
    if (i % 2 == 0) { // even: left then right
        wait(chopstick[i]);
        wait(chopstick[(i+1)%5]);
    } else { // odd: right then left
        wait(chopstick[(i+1)%5]);
        wait(chopstick[i]);
    }

    // ---- Eat ----

    if (i % 2 == 0) {
        signal(chopstick[(i+1)%5]);
        signal(chopstick[i]);
    } else {
        signal(chopstick[i]);
        signal(chopstick[(i+1)%5]);
    }

    // ---- Think ----
} while (true);
```

---

# Question 3: Test-and-Set Instruction: Explain with pseudocode how the Test and Set achieves mutual exclusion and progress while failing to attain bounded-waiting. 

There is a **shared lock variable** which can be either two values:
  - `0` → unlocked
  - `1` → locked

Before a process enters its **critical section**, it checks the lock:
  - If locked → it waits (busy waiting / spinlock)
  - If free → it acquires the lock and enters the critical section

**Atomic Instruction:**
```c
bool test_and_set(bool *target) {
    bool rv = *target;
    *target = true;
    return rv;
}
```

**Spinlock/Busy wait using test-and-set:**

```c
bool lock = false;

do {
    /*
     * test_and_set(&lock) is an atomic operation:
     * 1. Reads the current value of lock (rv)
     * 2. Sets lock = true (marks it as taken)
     * 3. Returns the rv value
     *
     * - If rv == false → we acquired the lock (enter critical section)
     * - If rv == true  → someone else has the lock, keep spinning
     */
    while (test_and_set(&lock)); // spin until lock is free

    /* --- critical section --- */

    lock = false; // release

    /* --- remainder section --- */
} while (true);
```

- **Mutual Exclusion:** The atomic `test_and_set` operation ensures that only one process can change `lock` from `false` to `true`. That process enters the critical section, while all others see `lock == true` and must wait. This guarantees that no two processes are ever in the critical section at the same time.

- **Progress:** When the lock is free (`lock == false`) and some processes want to enter, one of them will always succeed immediately by atomically flipping the lock to `true`. This ensures the system makes forward progress and does not get stuck waiting on processes that are not competing.

- **Bounded Waiting:** Test-and-Set does not guarantee fairness. A process waiting for the lock may be bypassed repeatedly if other processes manage to acquire the lock first every time it is released. This means starvation is possible, since there is no bound on how many times others can enter before the waiting process.
