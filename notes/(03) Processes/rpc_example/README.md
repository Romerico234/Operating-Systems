# Overview

This example demonstrates RPC communication with arithmetic operations:

- **rpc_server.cpp**: RPC server that provides arithmetic operations (add, sub, mul, div)
- **rpc_client.hpp**: Header file defining the RPC client interface
- **rpc_client.cpp**: RPC client implementation with connection and procedure call functions
- **client.cpp**: Example client program that demonstrates RPC usage

### How it works:
1. The RPC server creates a socket and listens for client connections
2. The server maintains a registry of available procedures (add, sub, mul, div)
3. Clients connect to the server and send procedure calls with arguments
4. The server executes the requested procedure and returns the result
5. The client receives the result as if it were a local function call

## Prerequisites

- C++ compiler (g++, clang++, etc)
- Unix OS (Linux, macOS, etc)

## Compilation Instructions

### Compile all components:
```bash
# RPC server
g++ -o rpc_server rpc_server.cpp

# RPC client library
g++ -c rpc_client.cpp

# Example client
g++ -o client client.cpp rpc_client.o
```

## Execution Instructions

### Start the RPC Server
In your first terminal window:
```bash
cd rpc_example/
./rpc_server
```

You should see:
```
RPC server listening on port 8080
```

### Start the RPC Client
In a second terminal window:
```bash
cd rpc_example/
./client
```

### Expected Output

**Server terminal:**
```
RPC server listening on port 8080
Client connected from 127.0.0.1
RPC server shutting down
```

**Client terminal:**
```
Connected to RPC server at 127.0.0.1:8080
add(5, 7) = 12
sub(20, 4) = 16
mul(6, 9) = 54
div(20, 5) = 4
div(10, 0) failed (division by zero)
RPC client finished
```

## RPC Protocol

### Request Format:
```
PROCEDURE_NAME arg1 arg2
```

### Response Format:
```
OK result_value          # Success
ERR error_message        # Error
```

### Available Procedures:
- `add a b` - Addition: a + b
- `sub a b` - Subtraction: a - b  
- `mul a b` - Multiplication: a * b
- `div a b` - Division: a / b (returns error for division by zero)