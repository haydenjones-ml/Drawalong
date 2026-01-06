#ifndef NET_H
#define NET_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdint.h>
#include <stdbool.h>

#pragma comment(lib, "ws2_32.lib")

typedef struct __attribute__((packed)) {
    uint8_t r, g, b, a;
    uint8_t type; // 0 = draw, 1 = erase
    uint8_t brush_size;
    int16_t x1, y1;
    int16_t x2, y2;
} Packet;

bool net_system_init();
void net_system_cleanup();
SOCKET net_init_host(const char* port);
SOCKET net_init_client(const char* host, const char* port);
int net_send_packet(SOCKET socket, Packet* packet);
int net_receive_packet(SOCKET socket, Packet* packet);
void net_close_socket(SOCKET socket);

#endif