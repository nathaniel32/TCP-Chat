#include "../include/client.h"

int main(int argc, char* argv[]) {

    if(argc < 4) {
        printf("Usage: %s <Ipv6- Adresse> <Port> <S-Nummer>", argv[0]);
        exit(1);
    }

    const char* IP = argv[1];
    const char* PORT = argv[2];
    char myid[10];
    strncpy(myid, argv[3], sizeof(myid) - 1);
    
    f_wsadata();
    
    struct addrinfo* result = NULL;
    struct addrinfo hints;

    // Die addrinfo-Struktur wird von der getaddrinfo-Funktion verwendet, um Hostadresseninformationen zu enthalten.    

    SOCKET clientSocket;

    memset(&hints, 0, sizeof(hints));  // set hints byte = 0, oder kann auch mit ZeroMemory nutzen.
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    result = f_getaddrinfo(IP, PORT, hints, result);
    clientSocket = f_socket();

    f_connToServer(clientSocket, result);
    freeaddrinfo(result);

    f_chat(clientSocket, myid);

    SOCKET sockets[SOCKET_NUM];
    sockets[0] = clientSocket;
    f_close(sockets, SOCKET_NUM);

    return 0;
}