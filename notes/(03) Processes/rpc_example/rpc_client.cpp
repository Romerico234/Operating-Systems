#include "rpc_client.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// Generic RPC call function that handles communication with the server
static bool call_rpc(int fd, const char* name, long long a, long long b, long long& out) {
    // Format request as "PROC_NAME a b" (server expects procedure name + arguments)
    char req[64];
    snprintf(req, sizeof(req), "%s %lld %lld", name, a, b);
    
    // Send request to server
    if (write(fd, req, strlen(req)) < 0) {
        return false; // Failed to send request
    }
    
    // Read response from server
    char buf[1024];
    ssize_t r = read(fd, buf, sizeof(buf) - 1);
    if (r <= 0) {
        return false; // Failed to read response or connection closed
    }
    
    buf[r] = '\0'; // null-terminate the response
    
    // Parse response: expect "OK result_value"
    return sscanf(buf, "OK %lld", &out) == 1;
}

// Connect to RPC server at specified IP and port
bool rpc_connect(RpcClient& c, const char* ip, uint16_t port) {
    // Create TCP socket
    c.fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (c.fd < 0) {
        return false; // Failed to create socket
    }
    
    // Configure server address structure
    sockaddr_in addr{};
    addr.sin_family = AF_INET; // IPv4 address family
    addr.sin_port = htons(port); // Convert port to network byte order
    
    // Convert IP address from text to binary format
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        return false; // Invalid IP address
    }
    
    // Attempt to connect to the server
    return ::connect(c.fd, (sockaddr*)&addr, sizeof(addr)) == 0;
}

// Close RPC connection and send quit signal to server
void rpc_close(RpcClient& c) {
    if (c.fd >= 0) {
        // Send quit signal to server
        write(c.fd, "quit\n", 5);
        
        // Close socket and invalidate file descriptor
        close(c.fd);
        c.fd = -1;
    }
}

// RPC wrapper functions for arithmetic operations
// Each function calls the generic call_rpc with the appropriate procedure name

bool rpc_add(RpcClient& c, long long a, long long b, long long& out) { 
    return call_rpc(c.fd, "add", a, b, out); 
}

bool rpc_sub(RpcClient& c, long long a, long long b, long long& out) { 
    return call_rpc(c.fd, "sub", a, b, out); 
}

bool rpc_mul(RpcClient& c, long long a, long long b, long long& out) { 
    return call_rpc(c.fd, "mul", a, b, out); 
}

bool rpc_div(RpcClient& c, long long a, long long b, long long& out) { 
    return call_rpc(c.fd, "div", a, b, out); 
}
