#ifndef CLIENT_H
#define CLIENT_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ws2tcpip.h>
#include <conio.h>
#include "../../common/include/packet.h"

#define MAX_MESSAGE_LENGTH 1024
#define MAX_BUFFER_LENGTH 1024

// Function declarations
void f_wsadata();
SOCKET f_clientSocket(struct addrinfo* result);
void f_connToServer(SOCKET clientSocket, struct addrinfo* result);
struct addrinfo* f_getaddrinfo(const char* IP, const char* PORT, struct addrinfo hints, struct addrinfo* result);
void f_delete(int index);
void f_chat(SOCKET clientSocket, char myid[]);
void f_close(SOCKET clientSocket);

#endif // CLIENT_H