#include "../include/server.h"

void f_wsadata() {
    // Winsock Initialisieren
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // Ergebnis in wsaData gespeichert
        perror("Failed to initialize Winsock");
        exit(EXIT_FAILURE);
    }
}

SOCKET f_serverSocket() {
    // ein TCP Socket bauen
    SOCKET fserverSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); 
    // AF_INET6 : Die IPv6-Adressfamilie (Internet Protocol Version 6).
    // SOCK_STREAM : Dieser Sockettyp verwendet tcp.
    // IPPROTO_TCP : Dies ist ein möglicher Wert, wenn der af-Parameter AF_INET oder AF_INET6 und der type-Parameter SOCK_STREAM ist.

    if (fserverSocket == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Server socket created successfully...\n");
    return fserverSocket;
}

struct addrinfo* f_getaddrinfo(const char* IP, const char* PORT, SOCKET serverSocket, struct addrinfo* result, struct addrinfo hints) {
    // Adressinformationen von Gastgebern und Diensten zu erhalten
    // Ergebnis wird in result speichert
    if (getaddrinfo(IP, PORT, &hints, &result) != 0) {
        perror("getaddrinfo failed");
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return result;
}

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