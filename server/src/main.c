#include "../include/server.h"

int main(int argc, char* argv[]) {
    if(argc < 4) {
        printf("Usage: %s <Ipv6- Adresse> <Port> <S-Nummer>", argv[0]);
        exit(1);
    }

    char myid[10];
    const char* IP = argv[1];
    const char* PORT = argv[2];
    strncpy(myid, argv[3], sizeof(myid) - 1);

    SOCKET serverSocket;
    SOCKET newSocket;
    struct addrinfo* result = NULL;
    struct addrinfo hints;
    // Die addrinfo-Struktur wird von der getaddrinfo-Funktion verwendet, um Hostadresseninformationen zu enthalten.

    f_wsadata();

    serverSocket = f_socket();

    memset(&hints, 0, sizeof(hints)); // set hints byte = 0, oder kann auch mit ZeroMemory nutzen
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    result = f_getaddrinfo(IP, PORT, hints, result);

    f_bind(PORT, serverSocket, result);

    freeaddrinfo(result);

    f_listen(serverSocket);

    newSocket = f_acc(serverSocket);

    f_chat(newSocket, myid);

    SOCKET sockets[SOCKET_NUM];
    sockets[0] = newSocket;
    sockets[1] = serverSocket;
    f_close(sockets, SOCKET_NUM);

    return 0;
}