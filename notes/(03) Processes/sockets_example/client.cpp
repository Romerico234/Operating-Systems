// client.cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0; // file descriptor for the client's socket
    struct sockaddr_in serv_addr; // structure that hlds the server's IP + Port info
    char buffer[BUFFER_SIZE] = {0}; // Space to store server's reply
    char *hello = (char *)"Hello from client"; // message the client will send

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4/IPv6 addresses from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/Address not supported\n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    // Send message to server
    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent to server\n");

    // Read response
    ssize_t nread = read(sock, buffer, BUFFER_SIZE);
    if (nread < 0) {
        printf("Read failed\n");
    } else {
        printf("Message from server: %s\n", buffer);
    }

    close(sock);
    return 0;
}