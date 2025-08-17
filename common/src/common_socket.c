#include "../include/common.h"

void f_wsadata() {
    // Winsock Initialisieren
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // Ergebnis in wsaData gespeichert
        perror("Failed to initialize Winsock");
        exit(EXIT_FAILURE);
    }
}

SOCKET f_socket() {
    SOCKET fclientSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); 

    if (fclientSocket == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Socket created successfully...\n");
    return fclientSocket;
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

void f_close(SOCKET sockets[], int count) {
    printf("Closing Connections...\n");

    for (int i = 0; i < count; i++) {
        if (sockets[i] != INVALID_SOCKET) {
            closesocket(sockets[i]);
            printf("Socket %d closed.\n", i);
        }
    }

    WSACleanup();
    printf("WSACleanup done.\n");
}