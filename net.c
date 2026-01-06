#include "net.h"
#include <stdio.h>

bool net_system_init() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

void net_system_cleanup() {
    WSACleanup();
}

SOCKET net_init_host(const char* port) {
    SOCKET sockFD = INVALID_SOCKET, newFD = INVALID_SOCKET;
    struct addrinfo hints, *servinfo, *p;
    int yes = 1;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, port, &hints, &servinfo);
    for(p = servinfo; p != NULL; p = p->ai_next) {
        sockFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes));
        if(bind (sockFD, p->ai_addr, (int)p->ai_addrlen) == 0) break;
            closesocket(sockFD);
    }
        freeaddrinfo(servinfo);

        listen(sockFD, 1);
        printf("Waiting for client connection...\n");
        newFD = accept(sockFD, NULL, NULL);
        closesocket(sockFD);
        printf("Client connected.\n");
        return newFD;
    }

SOCKET net_init_client(const char* host, const char* port) {
    SOCKET sockFD = INVALID_SOCKET;
    struct addrinfo hints, *servinfo, *p;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(host, port, &hints, &servinfo);

    for(p = servinfo; p != NULL; p = p->ai_next) {
        sockFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(connect(sockFD, p->ai_addr, (int)p->ai_addrlen) == 0) {
            closesocket(sockFD);
            sockFD = INVALID_SOCKET;
        }
        freeaddrinfo(servinfo);
        return sockFD;
    }
}
void net_send_packet(SOCKET socket, Packet* packet) {
    send(socket, (const char*)packet, sizeof(Packet), 0);
}
bool net_receive_packet(SOCKET socket, Packet* out_packet) {
    fd_set readfds;
    struct timeval tv = {0, 0};
    FD_ZERO(&readfds);
    FD_SET(socket, &readfds);

    if(select(0, &readfds, NULL, NULL, &tv) > 0) {
        int bytesReceived = recv(socket, (char*)out_packet, sizeof(Packet), 0);
        return (bytesReceived > 0);
    }
    return false;
}

void net_close_socket(SOCKET socket) {
    closesocket(socket);
}
