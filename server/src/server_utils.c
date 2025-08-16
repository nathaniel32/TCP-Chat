#include "../include/server.h"

void f_close(SOCKET newSocket, SOCKET serverSocket) {
    printf("Closing Connection...\n");

    closesocket(newSocket); // Die Closesocket-Funktion schließt einen vorhandenen Socket.
    closesocket(serverSocket);
    WSACleanup(); 
    // Für jeden erfolgreichen Aufruf von WSAStartup muss ein Aufruf von WSACleanup erfolgen.
    // WSACleanup-Funktionsaufruf führt die eigentliche Bereinigung aus.
}