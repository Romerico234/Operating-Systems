#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("Starting program. Initial PID: %d\n", getpid());
    int fd[2]; // file descriptors for pipe (fd[0] = read end, fd[1] = write end)
    
    // Create pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }
    
    // Fork process
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork error");
        return 1;
    } else if (pid == 0) { 
        // child process
        printf("Child process created! PID: %d\n", getpid());
        close(fd[1]); // close write end in child
        char buf[128]; // buffer to store received data
        ssize_t n = read(fd[0], buf, sizeof(buf) - 1); // read from pipe
        
        if (n < 0) { 
            perror("read"); 
            exit(1); 
        }
        
        buf[n] = '\0'; // null-terminate the string
        printf("child received: %s\n", buf);
        printf("%d\n", fd[0]); // print file descriptor
        close(fd[0]); // close read end
        exit(0);
    } else { 
        // parent process
        printf("Parent process PID: %d, Child PID: %d\n", getpid(), pid);
        close(fd[0]); // close read end in parent
        const char *msg = "hello from parent"; // message to send
        
        if (write(fd[1], msg, strlen(msg)) < 0) { 
            perror("write"); 
        }
        
        printf("Parent finished writing: %s\n", msg);
        close(fd[1]); // send EOF to child
        wait(NULL); // reap child
    }
    
    return 0;
}
