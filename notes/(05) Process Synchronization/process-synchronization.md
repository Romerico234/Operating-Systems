# Process Synchronization

## Background
- **Independent Process**: Processes that execute without depending on others  
- **Cooperating Process**: Processes that depends on or interacts with other processes or shares resources (memory, files, messages)  
- Cooperation enables parallelism but introduces synchronization challenges.

## Why Synchronization is Needed
- **Parallel execution**: Processes/threads can be interrupted at any time  
- **Shared data**: Concurrent access can lead to **data inconsistency**  

Maintaing data consistency requires mechanisms to ensure the *orderly execution* of cooperating processes.

### Example
Imagine you are working on a group project with one other student. Think of the two students as cooperating processes. Each student is responsible for 50% of the assignment, but if one fails to do their part, the final grade is affecte. 

Now suppose you spend four hours writing the introduction. When you open the shared Google Doc, you discover that your partner has already written their own introduction. Because you both worked on the same section without coordination, your effort was wasted. This illustrates the problem of concurrent access: processes must be aware of each other’s actions.

If you then paste your version of the introduction and overwrite what your partner wrote, the result is data inconsistency. One process (you) has changed the shared resource without the other process (your partner) being aware of it.

---

# Problems with Process Synchronization

## Race Condition
A **race condition** is a type of errors that occurs in concurrent computing when multiple processes or threads access or manipulate shared data simultaneously, and the final outcome depends on the order or timing of their execution.

### Example
`counter++` could be implemented as:
```
register1 = counter
register1 = register1 + 1
counter = register1
``` 

`counter--` could be implemented as:
```
register2 = counter
register2 = register2 - 1
counter = register2
``` 

If interleaved:
```
S0: P1 execute register1 = counter         {register1 = 5}
S1: P1 execute register1 = register1 + 1   {register1 = 6}
S2: P2 execute register2 = counter         {register2 = 5}
S3: P2 execute register2 = register2 - 1   {register2 = 4}
S4: P1 execute counter = register1         {counter = 6}
S5: P2 execute counter = register2         {counter = 4}
```

Final result = **4**  

> "Whoever loses the race" determines the final value.

## Producer–Consumer Problem (Bounded Buffer Problem)  
- **Producer**: Generates data and puts it into a buffer (a shared memory or data structure e.g. queue)  
- **Consumer**: Removes data from buffer for processing  
- **Buffer types**:
  - *Unbounded*: No practical limit on size
  - *Bounded*: Fixed size → requires synchronization

*Key Challenge*: The producer and consumer operate concurrently but because they share a buffer, synchronization mechanisms are needed to prevent issues like:

- **Overwriting** data (when the producer tries to add data to a full buffer)
- **Underflowing** (when the consumer tries to remove data from an empty buffer)

The challenge is to ensure that the producer does not produce data when the buffer is full and the consumer does not consume data when the buffer is empty.

*Solution*: The producer and consumer processes must be synchronized.

### Example

**Producer Code:**
```c
while (true) {
    /* produce an item in next_produced */

    while (counter == BUFFER_SIZE); /* do nothing */

    buffer[in] = next_produced;
    in = (in + 1) % BUFFER_SIZE;
    counter++;
}

/* Counter is incremented when a new item is added.
   Initially, counter variable = 0 */
```

**Producer Trace:**
```
| Step | Action                         | Buffer       | in | counter |
|------|--------------------------------|--------------|----|---------|
| 0    | Initial                        | [_, _, _]    | 0  | 0       |
| 1    | Produce A → insert at [0]      | [A, _, _]    | 1  | 1       |
| 2    | Produce B → insert at [1]      | [A, B, _]    | 2  | 2       |
| 3    | Produce C → insert at [2]      | [A, B, C]    | 0  | 3 (FULL)|
```

**Consumer Code:**
```c
while (true) {
    while (counter == 0); /* do nothing */

    next_consumed = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    counter--;

    /* consume the item in next_consumed */
}

/* Counter is decremented when a new item is removed.
   Initially, counter variable = 0 */

```

**Consumer Trace:**
```
| Step | Action                         | Buffer       | out | counter |
|------|--------------------------------|--------------|-----|---------|
| 0    | Initial                        | [A, B, C]   | 0   | 3        |
| 1    | Consume A ← remove from [0]    | [_, B, C]   | 1   | 2        |
| 2    | Consume B ← remove from [1]    | [_, _, C]   | 2   | 1        |
| 3    | Consume C ← remove from [2]    | [_, _, _]   | 0   | 0 (EMPTY)|
```

## Critical Section Problem
Consider a system of $n$ processes ${p_0, p_1, ... p_{n-1}}$. Each process $p_i$ has a segment of code, called a **critical section** in which $p_i$ is accessing or manipulating shared data.  

**Requirement**: When one process is executing in its critical section, no other process is allowed to execute their critical sections simultaneously.

### General Structure of Process $P$
The critical section problem is to design a **protocol** that ensures processes can safely share resources.

```
do {
entry section // Request to enter
critical section // Shared resource access
exit section // Release access
remainder section // Non-critical work
} while (true);
```
- Each process must ask permission to enter its critical section
- The section of code making this request is called the entry section
- The critical section may be followed by an exit section
- The remaining code is called the remainder section


### Solution to Critical Section Problem

**Mutual Exclusion:** If process $P_i$ is executing in its critical seciton, then no other processes can be executing in their critical sections.

**Progress:** If no process is executing in its critical section and there exist some processes that wish to enter their critical section, then the selection of the processes that will enter the
critical section next cannot be postponed indefinitely.

**Bounded Waiting:** A bound must exist on the number of times that other processes are allowed to enter their critical sections after a process has made a request to enter its critical section and before that request is granted.
- Assume that each process executes at a nonzero speed
- No assumption concerning relative speed of the $n$
processes

Additionally, the OS handles the critical section problem differently depending on whether the kernel is preemptive or non-preemptive:
- **Preemptive:** Allows preemption of process when running in kernel mode.
- **Non-preemptive:** Runs until exits kernel mode, blocks, or voluntarily yields CPU. Essentially free of race conditions in kernel mode.

---


# TBU (TO BE UNDERSTOOD)

### Peterson’s Solution
A classic software-based solution to synchronization problem for the critical section (in the two-process case). It satisfies all three requirements: mutual exclusion, progress, and bounded waiting.  

#### Shared Variables
- **int turn** → Indicates whose turn it is between the two processes to enter the critical section  
- **boolean flag[2]** → Marks if a process is ready to enter  
  - `flag[i] = true` → Process $P_i$ wants to enter  

#### Algorithm (for Process $P_i$)

```c
do {
    flag[i] = true;     // P_i is ready
    turn = j;           // Give priority to P_j
    while (flag[j] && turn == j);  // Busy wait if P_j also wants CS

    // ---- Critical Section ----

    flag[i] = false;    // P_i leaves CS
    // ---- Remainder Section ----
} while (true);

```

#### Example Walkthrough

Suppose we have two processes $P_0$ and $P_1$.

**Both want to enter CS:**
- $P_0$: sets `flag[0] = true`, then `turn = 1`
- $P_1$: sets `flag[1] = true`, then `turn = 0`

**Who enters first?**
- If both set flags, the **last write to `turn` decides**.
- Suppose `turn = 1` → $P_0$ waits (because it gave priority to $P_1$).
- $P_1$ enters the CS.

**After $P_1$ exits:**
- $P_1$ sets `flag[1] = false`.
- $P_0$ sees `flag[1] = false` → enters CS immediately.


#### Why Peterson’s Works

1. **Mutual Exclusion**
   - $P_i$ enters CS only if `flag[j] = false` **OR** `turn = i`
   - ⇒ Both cannot be in CS at the same time

2. **Progress**
   - If neither is in CS, whichever process sets its flag and gets the turn proceeds  
   - No indefinite postponement  

3. **Bounded Waiting**
   - Once $P_i$ finishes, $P_j$ is guaranteed to enter next if waiting  


### Test and Set

A hardware solution to the synchronization problem by using an **atomic instruction**.

#### Key Idea
- A **shared lock variable** can take values:
  - `0` → unlocked
  - `1` → locked
- Before entering the **critical section**, a process checks the lock:
  - If locked → it waits (busy waiting / spinlock)
  - If free → it acquires the lock and enters the critical section

#### `test_and_set` Instruction

```c
boolean test_and_set (boolean *target) {
    boolean rv = *target;
    *target = TRUE;
    return rv;
}
```

**Properties:**
1. Executed atomically (uninterruptible)
- An **atomic operation** is indivisible:  
  - It cannot be interrupted during execution  
  - Ensures that only one process at a time can modify the lock variable 
2. Returns the original value of target
3. Sets the new value of target to TRUE

#### Algorithm using `test_and_set()`
```c
do {
   while (TestAndSet(&lock)); // busy wait if lock = TRUE
   // critical section
   lock = FALSE;              // release lock
   // remainder section
} while (TRUE);

```

**Correctness:**
- Mutual Exclusion: Preserved (only one process gets lock = FALSE)
- Bounded Waiting: ❌ Not guaranteed. A process may be repeatedly bypassed by others while waiting thus starvation is possible.


#### Bounded-Waiting Mutual Exclusion with `test_and_set()`
Enhanced version ensures fairness by maintaing a waiting array and passing the lock in round-robin.

```c
do {
    waiting[i] = true;
    key = true;
    while (waiting[i] && key)
        key = test_and_set(&lock);

    waiting[i] = false;

    /* critical section */

    j = (i + 1) % n;
    while ((j != i) && !waiting[j])
        j = (j + 1) % n;

    if (j == i)
        lock = false;
    else
        waiting[j] = false;

    /* remainder section */
} while (true);

```

### Mutex Locks

### Semaphore

Disadvantages (as with the previous solutions) are the busy wait which is just running an infinte loop and wasting CPU cycles.

## Deadlock and Starvation
A **deadlock** is when two or more processes are waiting indefinitely for an event that can be caused by only one of the waiting processes.

Example:
Let $S$ and $Q$ be two semaphores initialized to 1:

**Starvation** is not deadlock. Say there are 10 processes. Everytime the CPU runs the process with highest privlelige. THe process with the least privelage gets less CPU time.

P1
|
v
P2
|
v
P10

Then, P11 comes and has higher priority than P10

Again this is not deadlock.

# Classical Problems of Synchronization

## Bounded-Buffer Problem
There is a buffer of $n$ slots and each slots can store one unit of data. There are two processes running, namely Producer and Consumer, which are operating on the buffer. The pro

## Readers and Writers Problem

## Dining-Philosophers Problem
