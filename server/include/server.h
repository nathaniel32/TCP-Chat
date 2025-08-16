#ifndef SERVER_H
#define SERVER_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ws2tcpip.h>
#include <conio.h>
#include "../../common/include/packet.h"

#define MAX_MESSAGE_LENGTH 1024
#define MAX_BUFFER_LENGTH 1024

// Function declarations
void f_wsadata();
SOCKET f_serverSocket();
struct addrinfo* f_getaddrinfo(const char* IP, const char* PORT, SOCKET serverSocket, struct addrinfo* result, struct addrinfo hints);
void f_bind(const char* PORT, SOCKET serverSocket, struct addrinfo* result);
void f_listen(SOCKET serverSocket);
SOCKET f_acc(SOCKET serverSocket);
void f_delete(int index);
void f_chat(SOCKET newSocket, char myid[]);
void f_close(SOCKET newSocket, SOCKET serverSocket);

#endif // SERVER_H