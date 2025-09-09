# Quiz 1

## What is the primary purpose of an interrupt in an operating system, and how does it impact the normal execution of processes?
Interrupts are the OS’s way of reacting to events rather than constantly checking devices. When an interrupt occurs, the CPU temporarily pauses the currrent process, saves its state, and transfers control to the appropriate interrupt service routine through the interrupt vector (a table storing the addresses of all service routines). 

In addition to hardware-generated interrupts (from devices), the OS also handles software-generated interrupts, called traps or exceptions. These occur when errors happen (e.g., divide by zero) or when a program explicitly requests OS services.

Because of this design, an operating system is said to be interrupt driven — it waits for events to occur and responds only when necessary.

## What is the purpose of caching in an operating system, and how does it enhance system performance?
A fundamental principle of computer systems is caching which is keeping data that is recently/frequently used in a faster, smaller storage so it can be accessed quickly instead of going to a slower memory or disk.  

When a program needs information, the system first checks the cache (fast storage).  
- If the data is found there (a cache hit), it is used immediately, which is much faster and contributes to lower latency
- If it is not present (a cache miss), the data is copied from slower storage (like RAM or disk) into the cache and then accessed  

Caches are always smaller than the storage they accelerate which makes cache management crucial. The OS and hardware must decide:  
- What size the cache should be  
- Which data to replace when the cache is full (replacement policies e.g., Least Recently Used)  

Note, caching is not limited to hardware. It happens at many levels: CPU registers, operating system buffers, and applications (like a web browser caching images).  


## What is context switching in an operating system, and why is it essential?
A context switch occurs when the CPU changes from executing one process to another. To do this safely, the system must save the state of the currently running process and later reload that state so the process can resume from the exact point it left off.  
- The saved information, known as the context, is stored in the Process Control Block (PCB). 

This enables multiprogramming or timesharing so the OS can share the CPU among many processes, keeping the system busy and fair.

Context switching is a form of overhead: while the system is saving and restoring states, no actual work (no user process execution) is being done. As a result, efficiency decreases with more frequent switches.  

The complexity of the OS and the size of the PCB directly influence how long a context switch takes. More complex systems (with larger contexts to save and restore) lead to longer switch times.  

## What is the primary purpose of the user interface in an operating system? List three key distinctions between Command-Line Interfaces (CLIs) and Graphical User Interfaces (GUls) in operating systems.
OSes “provide a user interface (CLI/GUI/Batch)” so users can interact with the system.

Three Distinctions:
- Input style: CLI is text-based command entry via a shell; GUI uses the desktop metaphor with icons and mouse actions (click, double-click, right-click).
- Implementation: A CLI may be built into the kernel or be a system program; GUI is a graphical environment layered on the OS (e.g., Aqua on UNIX).
- Accessibility: GUIs “made computers accessible to the masses,” whereas CLIs rely on typed commands and shells (Bash, Zsh, PowerShell).

## What is the main purpose of system calls in an operating system, and how do programs (user-level) utilize them to request services from the kernel?
System calls are the programming interface through which applications request services from the operating system. They provide a bridge between user programs and OS services and are typically written in high-level languages (C, C++).

Most programs access system calls indirectly through APIs rather than invoking raw system calls. Each system call is assigned a number. The System Call interface maintains an indexed table mapping numbers to system call routines. The system call interface invokes the intended system call in the OS Kernel and returns the status of the system call and any return values.

## How do system goals differ from user goals in operating system design, and can you provide an example of a user goal in this context?
- User goals emphasize convenience: the OS should be reliable, secure, easy to learn, and fast.
    - Example: Easy to learn → Provide a GUI with the desktop metaphor (icons, mouse actions) so users can launch programs and manage files without memorizing commands.
- System goals emphasize engineering efficiency: the OS should be easy to design, implement, and maintain, while remaining flexible and error-free.

## What is the fundamental difference between policy and mechanism in operating system design? Why is it important to separate policy from mechanism in OS design?
A key design principle is the separation of policy from mechanism.  
- Policy answers *what will be done* (the rule)
- Mechanism answers *how it will be done* (the tools)

Separating the two allows flexibility: policies can be changed without rewriting the mechanisms. For example, different scheduling policies can reuse the same low-level context-switch implementation. This separation turns OS design into a disciplined task of software engineering.

## What is SYSGEN, and how does it relate to operating system generation?
An OS must be configured for the specific hardware it runs on. A tool called SYSGEN gathers hardware details (CPU, memory, devices). Based on this, the system builds/generates a kernel tailored to the machine.

## How could a system be designed to allow a choice of operating systems from which to boot? What would the bootstrap program need to do?
Use a multi-stage boot with a loader like GRUB that “allows selecting kernels, versions, or boot parameters.” The bootstrap loader “initializes essential hardware” and locates and loads the kernel into memory; then the kernel initializes and runs.

## What is the main advantage of the layered approach to system design? What are the disadvantages of using the layered approach?
Main Advantage:
- Modularity & isolation: clear interfaces make the system easier to maintain and more reliable; changes/errors in one layer don’t propagate

Disadvantages:
- Overhead: extra crossings/indirections can hurt latency/throughput
- Rigidity: hard to do cross-layer optimizations; useful low-level hints can be hidden
- Boundary pain: picking the right layer cuts is hard; poor cuts cause workarounds
- End-to-end debugging: state spread across layers makes correlation harder
- Duplication: buffering/retries/caching may reappear in multiple layers