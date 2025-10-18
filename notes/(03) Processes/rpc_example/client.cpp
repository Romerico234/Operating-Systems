#include "rpc_client.hpp"
#include <stdio.h>

int main(int argc, char** argv) {
    // Get IP address from command line or use localhost
    const char* ip = (argc > 1) ? argv[1] : "127.0.0.1";
    
    // Create RPC client
    RpcClient c;
    
    // Connect to RPC server
    if (!rpc_connect(c, ip, 8080)) {
        fprintf(stderr, "connect failed\n");
        return 1;
    }
    
    printf("Connected to RPC server at %s:8080\n", ip);
    
    // Perform remote procedure calls
    long long result;
    
    // Test addition
    if (rpc_add(c, 5, 7, result)) {
        printf("add(5, 7) = %lld\n", result);
    }
    
    // Test subtraction
    if (rpc_sub(c, 20, 4, result)) {
        printf("sub(20, 4) = %lld\n", result);
    }
    
    // Test multiplication
    if (rpc_mul(c, 6, 9, result)) {
        printf("mul(6, 9) = %lld\n", result);
    }
    
    // Test division
    if (rpc_div(c, 20, 5, result)) {
        printf("div(20, 5) = %lld\n", result);
    }
    
    // Test division by zero (should fail)
    if (rpc_div(c, 10, 0, result)) {
        printf("div(10, 0) = %lld\n", result);
    } else {
        printf("div(10, 0) failed (division by zero)\n");
    }
    
    // Close connection
    rpc_close(c);
    printf("RPC client finished\n");
    
    return 0;
}
