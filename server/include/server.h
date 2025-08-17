#ifndef SERVER_H
#define SERVER_H

#define _CRT_SECURE_NO_WARNINGS

#include "../../common/include/packet.h"
#include "../../common/include/common.h"

#define SOCKET_NUM 2

// socket
void f_bind(const char* PORT, SOCKET serverSocket, struct addrinfo* result);
void f_listen(SOCKET serverSocket);
SOCKET f_acc(SOCKET serverSocket);

// chat
void f_chat(SOCKET newSocket, char myid[]);

#endif // SERVER_H