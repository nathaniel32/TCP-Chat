#ifndef COMMON_H
#define COMMON_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ws2tcpip.h> //SOCKET

// socket
void f_wsadata();
SOCKET f_socket();
struct addrinfo* f_getaddrinfo(const char* IP, const char* PORT, struct addrinfo hints, struct addrinfo* result);
void f_close(SOCKET sockets[], int count);

// utils
void f_delete(int index);

#endif