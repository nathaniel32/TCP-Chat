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

    /* u_long on= 1;                 //nonblocking Modus aktiviert
    int res; */

    SOCKET serverSocket;
    SOCKET newSocket;
    struct addrinfo* result = NULL;
    struct addrinfo hints;
    // Die addrinfo-Struktur wird von der getaddrinfo-Funktion verwendet, um Hostadresseninformationen zu enthalten.

    f_wsadata();

    serverSocket = f_serverSocket();

    memset(&hints, 0, sizeof(hints)); // set hints byte = 0, oder kann auch mit ZeroMemory nutzen
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    result = f_getaddrinfo(IP, PORT, serverSocket, result, hints);

    f_bind(PORT, serverSocket, result);

    freeaddrinfo(result);

    /* res = ioctlsocket(serverSocket, FIONBIO, &on);                       //Serversocket wird auf nonblocking gesetzt
    if (res != NO_ERROR)
        printf("ioctlsocket failed with error: %ld\n", res); */

    f_listen(serverSocket);

    newSocket = f_acc(serverSocket);

    f_chat(newSocket, myid);

    f_close(newSocket, serverSocket);

    return 0;
}