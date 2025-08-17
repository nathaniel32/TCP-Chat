#ifndef CLIENT_H
#define CLIENT_H

#define _CRT_SECURE_NO_WARNINGS

#include "../../common/include/packet.h"
#include "../../common/include/common.h"

#define MAX_MESSAGE_LENGTH 1024
#define MAX_BUFFER_LENGTH 1024
#define SOCKET_NUM 1

//socket
void f_connToServer(SOCKET clientSocket, struct addrinfo* result);

//chat
void f_chat(SOCKET clientSocket, char myid[]);

#endif // CLIENT_H