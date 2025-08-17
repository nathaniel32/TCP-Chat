#include "../include/server.h"

void f_bind(const char* PORT, SOCKET serverSocket, struct addrinfo* result) {
    // Wird verwendet, um einen Socket einer bestimmten Adresse und einem bestimmten Port zuzuordnen (zu binden).
    if (bind(serverSocket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR) {
        // Die Bindfunktion ordnet eine lokale Adresse einem Socket zu.
        
        perror("Bind failed");
        closesocket(serverSocket);
        freeaddrinfo(result);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Bind to Port Number %s \n", PORT);
}

void f_listen(SOCKET serverSocket) {
    // Akzeptieren Sie eingehende Verbindungen
    // SOMAXCONN = min 1 bis 2147483647 , <ws2tcpip.h>
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) { // SOMAXCONN für die maximale Länge der Warteschlange der ausstehenden Verbindungen.
        // Die Listenfunktion versetzt einen Socket in einen Zustand, in dem er auf eine eingehende Verbindung lauscht.
        perror("Listen failed");
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Listening...\n");
}

SOCKET f_acc(SOCKET serverSocket) {
    // um eingehende Verbindungen von Clients zu akzeptieren.
    struct sockaddr_in6 newAddr; // IPv6 Addresseinformation zu speichern
    int addr_size = sizeof(newAddr); // Speichergröße von newAddr
    SOCKET fnewSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addr_size);
    // Die Accept-Funktion lässt einen eingehenden Verbindungsversuch für einen Socket zu.
    // empfangene Informationen von accept() werden gespeichert in newAddr und addr_size.

    if (fnewSocket == INVALID_SOCKET) {
        perror("Accept failed");
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Connected\n");

    return fnewSocket; // Nutzung:  Verbindung mit Client
}