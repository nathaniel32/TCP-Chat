#include "../include/client.h"

int main(int argc, char* argv[]) {

    if(argc < 4) {
        printf("Usage: %s <Ipv6- Adresse> <Port> <S-Nummer>", argv[0]);
        exit(1);
    }

    /* u_long on= 1;                 //nonblocking Modus aktiviert
    int res; */

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
    clientSocket = f_clientSocket(result);

    f_connToServer(clientSocket, result);
    freeaddrinfo(result);

    /* res = ioctlsocket(clientSocket, FIONBIO, &on);                   //Clientsocket wird auf nonblocking gesetzt       
    if (res != NO_ERROR)
        printf("ioctlsocket failed with error: %ld\n", res); */

    f_chat(clientSocket, myid);

    f_close(clientSocket);

    return 0;
}