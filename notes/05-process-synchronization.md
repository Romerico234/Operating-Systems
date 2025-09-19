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


### TBD...