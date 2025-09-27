// server.cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // POSIX functions
#include <arpa/inet.h> //

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket; // file descriptors for sockets, server_fd is the listening socket, new_socket is for a connected client 
    struct sockaddr_in address; // holds IP/port info
    int addrlen = sizeof(address); 
    char buffer[BUFFER_SIZE] = {0}; // creates a buffer of 1024 bytes initialized to all zeros to store incoming messages from the client
    char *hello = (char *)"Hello from server"; // string literal to send as a response

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { //AF_INET = IPv4 and SOCK_STREAM = TCP, 0 indicates to use default protocol (TCP)
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections on any local IP
    address.sin_port = htons(PORT); // Converts port number to network byte order (big-endian is standard for networking)

    // Bind assigns the socket server_fd to the given IP + Port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections, upto 3 pending connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept a client connection and blocks until a client connects and creates a new socket dedicated to that client
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) { 
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Read message from client upto 1024 bytes
    ssize_t nread = read(new_socket, buffer, BUFFER_SIZE);
    if (nread < 0) {
        perror("Read failed");
    } else {
        printf("Message from client: %s\n", buffer);
    }

    // Send reply to client
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent to client\n");

    close(new_socket);
    close(server_fd);
    return 0;
}