# CPU Scheduling

In a single-processor system, only one process can run at a time. Other processes must wait until the CPU becomes free. For example, a process runs until it must wait (often due to an I/O request). During I/O, the CPU sits idle and all the waiting time is wasted; no useful work is completed.

With **multiprogramming**, the OS can switch the CPU to another process during waiting to keep the system productive.

The **objective of multiprogramming** is to keep the CPU busy at all time, to **maximize CPU utilization**.

---

# CPU–I/O Burst Cycle

Process execution consists of a cycle of **CPU execution (CPU burst)** and **I/O wait (I/O burst)**.

<p align="center"> <img src="../../images/083.png" width="400"/> </p>

- **CPU burst**: The time spent when the process is in CPU execution  
- **I/O burst**: The time spent when the process is in I/O wait
- A process usually executes on the CPU first then usually needs to perform some I/O

> **Main Observation**: **CPU burst distribution** is of primary concern for scheduling.

---

# Preemptive and Non-Preemptive Scheduling
CPU scheduling can be categorized into two approaches based on whether the kernel is preemptive or not.
- **Prempetive:** The kernel allows a process to be preempted (interrupted and replaced) in kernel mode.  
- **Non-Preemptive:** Once a process enters kernel mode, it continues to execute until its exits, blocks, or voluntarily yields the CPU.  
    - Advantage: Essentially free of race conditions in kernel mode.  

## Short-term Scheduler
Whenever the CPU becomes idle, the OS must pick a process from the ready queue to execute. This selection is done by the **short-term scheduler**. The scheduler selects a process from the process in memory that are ready to execute and allocates the CPU to that process.
- In **non-preemptive systems**, it is invoked only when the current process **finishes** or **blocks**.  
- In **preemptive systems**, it can also be triggered by **interrupts** or **priority changes**, allowing the OS to switch processes even while one is running.  

## Dispatcher
The dispatcher module gives gives **control of the CPU** to the chosen process. It handles:
- Context switching  
- Switching to user mode  
- Jumping to correct user instruction  

**Dispatch Latency**: The time taken to stop one process and start another.

Both **preemptive** and **non-preemptive** systems use the dispatcher, the difference is simply **how often** it is invoked.  

## CPU Decision Points

Scheduling occurs when a process:
1. Switches from **running → waiting**  
2. Switches from **running → ready** (interruption)  
3. Switches from **waiting → ready** (I/O completion)  
4. **Terminates**

### Types
- **Scheduling under Cases 1 & 4** → No choice in terms of scheduling, a new process in the ready queue must be selected for execution. Corresponds to **non-preemptive** scheduling events.  
- **Scheduling under Cases 2 & 3** → A process must be chosen from the ready queue based on some selection method. Corresponds to **preemptive** scheduling events.  

---

# Scheduling Criteria

1. **CPU Utilization:** We want to keep the CPU as busy as possible  
   - CPU utilization can range from 0–100%  
   - Lightly loaded systems utilizes up to ~40% CPU  
   - Heavily loaded systems utilizes up to ~90% CPU

2. **Throughput:** Number of processes that complete their execution per time unit

3. **Turnaround Time:** Amount of time to execute a particular process (from submission → completion) 
   - Includes wait time (into memory and in the ready queue), CPU execution, I/O

4. **Waiting Time:** Amount of time a process has been waiting in the ready queue  

5. **Response Time:** Amount of time it takes from when a request was submitted until the first response (not completion) is produced


## Scheduling Algorithm Optimization Criteria
- **Maximize**:
  - CPU utilization
  - Throughput
- **Minimize**:
  - Turnaround time
  - Waiting time
  - Response time
$P_2$ to $P_5$ all have the same priority. The process that requests the CPU first is allocated the CPU first. The implementation of FCFS policy is easily managed with a FIFO queue. When a process enters the ready queue, its PCB is linked onto the tail of the queue. When the CPU is free, it is allocated to the process at the head of the queue.

---

# Process Scheduling Algorithms

## First-Come First Served (FCFS) Scheduling

```
P1 → running  

    P2 ← P1   [P2 > P1]  
    P3 ← P1   [P3 > P1]  
    P4 ← P1   [P4 > P1]  
    P5 ← P1   [P5 > P1]  
```

Processes $P_2$ through $P_5$ come along all with the some priority greater than $P_1$. In this case, the CPU is allocated based on the **First-Come, First-Served (FCFS)** policy meaning the process that requests the CPU earliest will run first.  

**Key Idea:** The process that requests the CPU first is allocated the CPU first.

**Implementation:** 
- Implemented using a FIFO queue
- When a process enters the ready queue, its PCB is linked onto the tail of the queue
- When the CPU is free, it is allocated to the process at the head of the queue
- Default nature is **non-preemptive** (once a process starts execution, it cannot be interrupted until completion)

### Example 1

<p align="center"> <img src="../../images/084.png" width="500"/> </p>

### Example 2 

<p align="center"> <img src="../../images/085.png" width="500"/> </p>

## Shortest-Job-First (SJF) Scheduling
As seen in the previous examples, the average waiting time in FCFS scheduling can be quite high. To reduce this, a more efficient approach called **Shortest Job First (SJF)** scheduling was introduced.

**Key Idea:** Associate with each process the length of its **current/remaining CPU burst**. Then, we use these lengths to schedule the process with the **shortest CPU burst time** first. If two processes have the same burst time, **FCFS** is used to break the tie.
- Default nature is also **non-preemptive**

**Optimality:** SJF is **optimal** because it provides the **minimum average waiting time** for a given set of processes.

### Example 1

<p align="center"> <img src="../../images/086.png" width="500"/> </p>

### Example 2 

Up to this point, we’ve assumed that all processes arrive at the same time. However, in real systems, processes often **arrive at different times**. To account for this, we introduce the concept of **arrival time**, which represents when each process enters the ready queue. Let’s see how this affects the way the CPU schedules processes.

<p align="center"> <img src="../../images/087.png" width="500"/> </p>

### Example 3

<p align="center"> <img src="../../images/098.png" width="500"/> </p>

## Priority Scheduling

**Key Idea:**
1. Each process is assigned a **priority number** (an integer).
2. The CPU is allocated to the process with the **highest priority**.  
    - By convention, the **smallest integer = highest priority**.

**Implementation (Two Modes):**
  - **Preemptive Priority Scheduling:** If a new process arrives with higher priority, it preempts the currently running process
  - **Non-preemptive Priority Scheduling:** Once a process starts execution, it continues until completion (no preemption)

> SJF is essentially a special case of Priority Scheduling. In SJF, the priority is the **inverse of the predicted next CPU burst time** (shorter jobs are treated as higher priority).

**Problem:** Starvation as low-priority processes may never get CPU time if higher-priority processes keep arriving.  

**Solution:** Aging &mdash; as time progres, gradually increase the priority of processes that wait in the system for a long time so that low-priority processes eventually execute. 

### Example

<p align="center"> <img src="../../images/088.png" width="500"/> </p>

## Round Robin (RR) Scheduling
Each process gets a **small unit of CPU time** (called the **time quantum**, `q`), usually **10–100 milliseconds**. After this time has elapsed, the process is **preempted** and **added to the end of the ready queue**.

If there are `n` processes in the ready queue and the time quantum is `q`, each process gets `1/n` of the CPU time in chunks of at most `q` time units. No process waits more than **(n – 1) × q** time units.

A **timer interrupt** triggers every quantum to schedule the next process.
- **q large ⇒** behaves like FIFO (First-Come, First-Served)  
- **q small ⇒** must still be large relative to context-switch time, otherwise overhead is too high

### Fairness and Responsiveness
- Unlike other scheduling algorithms, Round Robin ensures that every process gets an equal share of the CPU
  - Avoids the scenario where long processes or low-priority tasks are left waiting for a long time
  - Ensures improved responsiveness — an essential feature in time-sharing systems

- **Avoiding Starvation:**  In Priority Scheduling, low-priority processes may suffer from starvation. Round Robin avoids this problem.

### Example 1

<p align="center"> <img src="../../images/089.png" width="500"/> </p>

- **Turnaround Time = Completion Time - Arrival Time**
- **Waiting Time = Turnaroundtime - Burst Time**

### Example 2

<p align="center"> <img src="../../images/099.png" width="500"

### Time Quantam and Context Switch Time

The **time quantum** (`q`) in RR scheduling determines how long each process runs before the CPU switches to the next one. But switching between processes isn’t free and each switch introduces a **context switch overhead** (time needed to save the current process state and load the next one).
y.

Let's look at the following example:

<p align="center"> <img src="../../images/092.png" width="500"/> </p>

Here, the total process time is fixed at $10ms$, but we vary the quantum.

1. **Quantum = $12ms$ (no preemption):**  
   - The process completes before its time slice expires  
   - Acts like FCFS scheduling  
   - 0 context switches, no overhead, but poor responsiveness if other processes are waiting

2. **Quantum = $6ms$ (moderate preemption):**  
   - The process executes for 6 units, then the CPU switches once to handle another process  
   - Only 1 context switch, meaning some responsiveness with minimal overhead  
   - This is typically a balanced configuration

3. **Quantum = $1ms$ (frequent preemption):**  
   - The process is interrupted every unit of time → 9 context switches  
   - This gives excellent responsiveness but terrible efficiency as CPU time is wasted on switching instead of actual work

Round Robin scheduling typically has a higher average turnaround time than SJF but provides better response time. The time quantum (`q`) should be large compared to the context switch time to minimize overhead, with `q` usually ranging from $10ms$ to $100ms$, while context switches typically take less than $10µs$.

## Multilevel Queue Scheduling (MLQ)
The **ready queue** is partitioned into **separate queues** e.g.:
  - **foreground** (interactive)
  - **background** (batch)

Foreground processes and background processes have:
 - Different response-time requirements  
- Different sci heduling needs

Each queue has its **own scheduling algorithm**:
  - **foreground – Round Robin (RR)**
  - **background – First-Come, First-Served (FCFS)**

The scheduling will take place **within** and **among** the queues.

### Scheduling Between / Among Queues
Again, scheduling must be done between/among the queues:
  - **Fixed priority scheduling** (i.e. serve all from foreground, then from background). → Possibility of starvation
  - **Time slice** – each queue gets a certain amount of CPU time which it can schedule among its processes (e.g. 80% to foreground (RR), 20% to background (FCFS))

### Example of Queue Structure
<p align="center"> <img src="../../images/090.png" width="500"/> </p>

## Multilevel Feedback Queue (MLFQ)
The **Multilevel Feedback Queue** improves on the basic MLQ by introducing flexibility as processes can move between various queues (aging can be implemented this way).

A **multilevel-feedback-queue scheduler** is defined by the following parameters:
  - Number of queues
  - Scheduling algorithms for each queue
  - Method used to determine when to upgrade a process
  - Method used to determine when to demote a process
  - Method used to determine which queue a process will enter when it needs service

### Example of Multilevel Feedback Queue

<p align="center"> <img src="../../images/091.png" width="500"/> </p>