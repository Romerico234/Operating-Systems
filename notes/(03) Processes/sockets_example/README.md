
# Overview

This example demonstrates basic TCP socket communication between a client and server process:

- **server.cpp**: A TCP server that listens on port 8080, accepts client connections, receives messages, and sends responses
- **client.cpp**: A TCP client that connects to the server, sends a message, and receives a response

### How it works:
1. The server creates a socket, binds it to port 8080, and listens for incoming connections
2. The client creates a socket and connects to the server at localhost:8080
3. The client sends "Hello from client" to the server
4. The server receives the message, prints it, and responds with "Hello from server"
5. The client receives and prints the server's response
6. Both processes close their sockets and exit

## Prerequisites

- C++ compiler (g++, clang++, etc)
- Unix OS (Linux, macOS, etc)

## Compilation Instructions

### Compile the server:
```bash
g++ -o server server.cpp
```

### Compile the client:
```bash
g++ -o client client.cpp
```

### Start the Server
In your first terminal window:
```bash
cd sockets_example/
./server
```

You should see:
```
Server listening on port 8080...
```

The server will wait for a client connection.

### Start the Client
In a second terminal window, navigate to the same directory and run:
```bash
cd sockets_example/
./client
```

### Expected Output

**Server terminal:**
```
Server listening on port 8080...
Message from client: Hello from client
Hello message sent to client
```

**Client terminal:**
```
Hello message sent to server
Message from server: Hello from server
```