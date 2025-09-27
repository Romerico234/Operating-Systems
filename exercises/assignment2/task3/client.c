#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#pragma comment(lib, "Ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#define PORT 8080
#define BUF 1024

int main(void) {
    WSADATA w;  WSAStartup(MAKEWORD(2,2), &w);

    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in srv = {0};
    srv.sin_family = AF_INET;
    srv.sin_port   = htons(PORT);
    srv.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 

    connect(s, (struct sockaddr*)&srv, sizeof(srv));

    const char *msg = "Hello from client";
    send(s, msg, (int)strlen(msg), 0);
    printf("Sent to server.\n");

    char buf[BUF];
    int n = recv(s, buf, BUF-1, 0);
    if (n > 0) { buf[n] = '\0'; printf("From server: %s\n", buf); }

    closesocket(s);
    WSACleanup();
    return 0;
}
