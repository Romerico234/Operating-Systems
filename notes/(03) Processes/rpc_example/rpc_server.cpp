#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFSZ 1024

// Function pointer type for procedures with 2 arguments
typedef int (*proc2_fn)(long long a, long long b, long long *out);

// Arithmetic operation implementations
static int add_impl(long long a, long long b, long long *out) { 
    *out = a + b; 
    return 0; 
}

static int sub_impl(long long a, long long b, long long *out) { 
    *out = a - b; 
    return 0; 
}

static int mul_impl(long long a, long long b, long long *out) { 
    *out = a * b; 
    return 0; 
}

static int div_impl(long long a, long long b, long long *out) { 
    if (b == 0) return -1; // division by zero error
    *out = a / b; 
    return 0; 
}

// Procedure structure to hold procedure information
typedef struct {
    const char *name; // procedure name (e.g. "add", "div")
    int arity; // number of arguments (valid when arity is 2)
    proc2_fn fn; // function pointer to implementation
} Proc;

// Registry of available RPC procedures
static Proc procs[] = {
    {"add", 2, add_impl},
    {"sub", 2, sub_impl},
    {"mul", 2, mul_impl},
    {"div", 2, div_impl}
};

// Function to find a procedure by name
static Proc* find_proc(const char *name) {
    size_t n = sizeof(procs)/sizeof(procs[0]);
    for (size_t i = 0; i < n; i++) {
        if (strcmp(procs[i].name, name) == 0) {
            return &procs[i];
        }
    }
    return NULL;
}

// Helper function to send a line over socket
static int send_line(int fd, const char *s) {
    size_t n = strlen(s);
    return (int)write(fd, s, n) == (int)n;
}

int main(void) {
    // Create socket
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) { 
        perror("socket"); 
        return 1; 
    }
    
    // Set socket option to reuse address
    int opt = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Configure server address
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces
    addr.sin_port = htons(PORT); // convert port to network byte order
    
    // Bind socket to address
    if (bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) { 
        perror("bind"); 
        return 1; 
    }
    
    // Start listening for connections
    if (listen(sfd, 1) < 0) { 
        perror("listen"); 
        return 1; 
    }
    
    printf("RPC server listening on port %d\n", PORT);
    
    // Accept client connection
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int cfd = accept(sfd, (struct sockaddr*)&client_addr, &client_len);
    if (cfd < 0) {
        perror("accept");
        return 1;
    }
    
    printf("Client connected from %s\n", inet_ntoa(client_addr.sin_addr));
    
    // Main RPC processing loop
    char buffer[BUFSZ];
    while (1) {
        // Read request from client
        ssize_t n = read(cfd, buffer, BUFSZ - 1);
        if (n <= 0) break; // client disconnected or error
        
        buffer[n] = '\0'; // null-terminate
        
        // Parse request: format is "PROC_NAME arg1 arg2"
        char proc_name[64];
        long long a, b;
        
        if (sscanf(buffer, "%63s %lld %lld", proc_name, &a, &b) != 3) {
            send_line(cfd, "ERR invalid_format\n");
            continue;
        }
        
        // Find the requested procedure
        Proc *p = find_proc(proc_name);
        if (p == NULL) {
            send_line(cfd, "ERR unknown_procedure\n");
            continue;
        }
        
        // Check arity (number of arguments)
        if (p->arity != 2) {
            send_line(cfd, "ERR wrong_arity\n");
            continue;
        }
        
        // Execute the procedure
        long long out = 0;
        int rc = p->fn(a, b, &out);
        
        if (rc == 0) {
            // Success - send result
            char line[64];
            snprintf(line, sizeof(line), "OK %lld\n", out);
            send_line(cfd, line);
        } else if (p == find_proc("div")) {
            // Special error handling for division by zero
            send_line(cfd, "ERR divide_by_zero\n");
        } else {
            // General execution error
            send_line(cfd, "ERR exec_failed\n");
        }
    }
    
    // Cleanup
    close(cfd);
    close(sfd);
    printf("RPC server shutting down\n");
    return 0;
}
