#include "../include/client.h"

void f_wsadata() {
    // Winsock Initialisieren
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // Ergebnis in wsaData gespeichert
        perror("Failed to initialize Winsock");
        exit(EXIT_FAILURE);
    }
}

SOCKET f_clientSocket(struct addrinfo* result) {
    // ein TCP Socket bauen
    SOCKET fclientSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    // AF_INET6 : Die IPv6-Adressfamilie (Internet Protocol Version 6).
    // SOCK_STREAM : Dieser Sockettyp verwendet tcp.
    // IPPROTO_TCP : Dies ist ein möglicher Wert, wenn der af-ParameterAF_INET oder AF_INET6 und der type-ParameterSOCK_STREAM ist.

    if (fclientSocket == INVALID_SOCKET) {
        perror("Client socket creation failed");
        freeaddrinfo(result);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Client socket created successfully...\n");
    return fclientSocket;
}

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

struct addrinfo* f_getaddrinfo(const char* IP, const char* PORT, struct addrinfo hints, struct addrinfo* result) {
    // Adressinformationen von Gastgebern und Diensten zu erhalten
    // Ergebnis wird in result speichert
    
    if (getaddrinfo(IP, PORT, &hints, &result) != 0) {
        perror("getaddrinfo failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return result;
}