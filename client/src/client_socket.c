#include "../include/client.h"

void f_connToServer(SOCKET clientSocket, struct addrinfo* result) {
    if (connect(clientSocket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR) {
        // Die Connect-Funktion stellt eine Verbindung mit einem angegebenen Socket her.
        
        fprintf(stderr, "Connection to server failed: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        freeaddrinfo(result);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Connected to server successfully...\n");
}