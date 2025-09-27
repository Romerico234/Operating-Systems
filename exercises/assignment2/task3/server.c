#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BUF 1024

int main(void) {
    WSADATA w;  WSAStartup(MAKEWORD(2,2), &w);

    SOCKET srv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(srv, (struct sockaddr*)&addr, sizeof(addr));
    listen(srv, 1);

    printf("Server listening on %d...\n", PORT);
    SOCKET cli = accept(srv, NULL, NULL);

    char buf[BUF];
    int n = recv(cli, buf, BUF-1, 0);
    if (n > 0) { buf[n] = '\0'; printf("From client: %s\n", buf); }

    const char *msg = "Hello from server";
    send(cli, msg, (int)strlen(msg), 0);

    closesocket(cli);
    closesocket(srv);
    WSACleanup();
    return 0;
}
