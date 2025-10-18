# Overview

This example demonstrates basic pipe communication between parent and child processes:

- **ordinaryPipe.cpp**: A program that creates a pipe, forks a child process, and demonstrates parent-child communication through the pipe

### How it works:
1. The parent process creates a pipe using `pipe()` system call
2. The parent forks a child process using `fork()`
3. The parent closes the read end of the pipe and writes "hello from parent" to the write end
4. The child closes the write end of the pipe and reads the message from the read end
5. The child prints the received message and exits
6. The parent waits for the child to complete and then exits

## Prerequisites

- C++ compiler (g++, clang++, etc)
- Unix OS (Linux, macOS, etc)

## Compilation Instructions

### Compile the pipe example:
```bash
g++ -o ordinaryPipe ordinaryPipe.cpp
```

## Execution Instructions

### Run the Pipe Example
In your terminal:
```bash
cd pipe_example/
./ordinaryPipe
```

### Expected Output

```
Parent finished writing: hello from parent
child received: hello from parent
3
```