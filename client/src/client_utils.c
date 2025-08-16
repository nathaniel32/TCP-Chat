#include "../include/client.h"

void f_close(SOCKET clientSocket) {
    printf("Closing Connection...\n");
    closesocket(clientSocket); // Die Closesocket-Funktion schließt einen vorhandenen Socket.
    WSACleanup();
}