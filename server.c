#define _CRT_SECURE_NO_WARNINGS

#include "packet.h"
#include <stdio.h>
#include <ws2tcpip.h> //winsock2-socket ist schon drin.
#include <conio.h>

#define MAX_MESSAGE_LENGTH 1024
#define MAX_BUFFER_LENGTH 1024

void f_wsadata(){
    //Winsock Initialisieren
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { //Ergebnis in wsaData gespeichert
        perror("Failed to initialize Winsock");
        exit(EXIT_FAILURE);
    }
}

SOCKET f_serverSocket(){
    //ein TCP Socket bauen
    SOCKET fserverSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); 
    //AF_INET6 : Die IPv6-Adressfamilie (Internet Protocol Version 6).
    //SOCK_STREAM : Dieser Sockettyp verwendet tcp.
    //IPPROTO_TCP : Dies ist ein möglicher Wert, wenn der af-Parameter AF_INET oder AF_INET6 und der type-Parameter SOCK_STREAM ist.

    if (fserverSocket == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Server socket created successfully...\n");
    return fserverSocket;
}

struct addrinfo* f_getaddrinfo(const char* IP, const char* PORT,SOCKET serverSocket, struct addrinfo* result, struct addrinfo hints){
    //Adressinformationen von Gastgebern und Diensten zu erhalten
    //Ergebnis wird in result speichert
    if (getaddrinfo(IP, PORT, &hints, &result) != 0) {
        perror("getaddrinfo failed");
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return result;
}

void f_bind(const char* PORT, SOCKET serverSocket, struct addrinfo* result){
    //Wird verwendet, um einen Socket einer bestimmten Adresse und einem bestimmten Port zuzuordnen (zu binden).
    if (bind(serverSocket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR) {
        //Die Bindfunktion ordnet eine lokale Adresse einem Socket zu.
        
        perror("Bind failed");
        closesocket(serverSocket);
        freeaddrinfo(result);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Bind to Port Number %s \n", PORT);
}

void f_listen(SOCKET serverSocket){
    //Akzeptieren Sie eingehende Verbindungen
    //SOMAXCONN = min 1 bis 2147483647 , <ws2tcpip.h>
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) { //SOMAXCONN für die maximale Länge der Warteschlange der ausstehenden Verbindungen.
        //Die Listenfunktion versetzt einen Socket in einen Zustand, in dem er auf eine eingehende Verbindung lauscht.
        perror("Listen failed");
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Listening...\n");
}

SOCKET f_acc(SOCKET serverSocket){
    //um eingehende Verbindungen von Clients zu akzeptieren.
    struct sockaddr_in6 newAddr; //IPv6 Addresseinformation zu speichern
    int addr_size = sizeof(newAddr); //Speichergröße von newAddr
    SOCKET fnewSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addr_size);
    //Die Accept-Funktion lässt einen eingehenden Verbindungsversuch für einen Socket zu.
    //empfangene Informationen von accept() werden gespeichert in newAddr und addr_size.

    if (fnewSocket == INVALID_SOCKET) {
        perror("Accept failed");
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Connected\n");

    return fnewSocket; //Nutzung:  Verbindung mit Client
}

void f_delete(int index){
    //delete text
    for(int a=1; a <= index+5; a++){
        printf("\b \b");
    }
}

void f_chat(SOCKET newSocket, char myid[]){
    char message[MAX_MESSAGE_LENGTH];
    char buf[MAX_BUFFER_LENGTH];

    //Parameter für select()
    fd_set read_fds;
    struct timeval timeout;

    int index = -1;
    char ch;

    while (1) {
        FD_ZERO(&read_fds); // Initialisiert den Satz auf den leeren Satz. Ein Set sollte vor der Verwendung immer gelöscht werden.
        FD_SET(newSocket, &read_fds); //Fügt Socket zum read_fds hinzu. Man kann mehrere Socket in read_fds speichern.

        timeout.tv_sec = 0; //seconds
        timeout.tv_usec = 0; //microseconds

        int ready_r = select(0, &read_fds, NULL, NULL, &timeout); //non blocking read

        if (ready_r == SOCKET_ERROR) {
            perror("select");
            break;
        }

        if (ready_r > 0 && FD_ISSET(newSocket, &read_fds)) { //Überprüfung der Verfügbarkeit der zu lesenden Daten.
            packet myPacket;

            int bytesReceived = recv(newSocket, (char*)&myPacket, sizeof(myPacket), 0);

            //flags (letzte Spalten) : Bitflags, die das Verhalten des Recv-Aufrufs steuern, wie MSG_PEEK,MSG_WAITALL, usw.

            if (bytesReceived > 0) {
                if (index >= 0) {
                    message[index] = '\0';
                    //printf("\r");
                    f_delete(index);
                    printf("%s: %s\n", myPacket.sNum, myPacket.text);
                    printf("You: %s", message);
                }else{
                    printf("%s: %s\n", myPacket.sNum, myPacket.text);
                }
            } else if (bytesReceived == 0) { //wenn Client f_close() gerufen hätte.
                printf("Connection closed by the client\n");
                break;
            } else {
                printf("Connection interrupted by the client\n");
                break;
            }
        }

        if (_kbhit()) { //wenn Benutzer etwas eingibt
            ch = _getch();
            if (ch == 8 && index > 0) {
                printf("\b \b"); //delete
                index--;
            } else if (ch == 8 && index == 0) {
                index = 0; //passiert nix da kein mehr message zu löschen
            } else {
                if(index == -1){
                    printf("You: ");
                    index = 0;
                }
                message[index++] = ch; //laenge von message zunehmend
                printf("%c", ch);
            }

            if (ch == '\r' || ch == '\n' || index >= sizeof(message) - 1) {
                message[index] = '\0'; //als string wechseln

                if(strcmp(message, "Quit\r") == 0){
                    break;
                }

                packet myPacket;
                strcpy(myPacket.sNum, myid); //snum speichern im paket
                myPacket.sNum[sizeof(myPacket.sNum) - 1] = '\0';

                strcpy(myPacket.text, message);
                myPacket.text[sizeof(myPacket.text) - 1] = '\0';

                send(newSocket, (void*)&myPacket, sizeof(myPacket), 0);

                printf("\n");
                index = -1;
            }
        }
    }
}

void f_close(SOCKET newSocket, SOCKET serverSocket){
    printf("Closing Connection...\n");

    closesocket(newSocket); //Die Closesocket-Funktion schließt einen vorhandenen Socket.
    closesocket(serverSocket);
    WSACleanup(); 
    //Für jeden erfolgreichen Aufruf von WSAStartup muss ein Aufruf von WSACleanup erfolgen.
    //WSACleanup-Funktionsaufruf führt die eigentliche Bereinigung aus.
}

int main(int argc,char* argv[]) {
    if(argc < 4){
        printf("Usage: %s <Ipv6- Adresse> <Port> <S-Nummer>",argv[0]);
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
    //Die addrinfo-Struktur wird von der getaddrinfo-Funktion verwendet, um Hostadresseninformationen zu enthalten.

    f_wsadata();

    serverSocket = f_serverSocket();

    memset(&hints, 0, sizeof(hints)); //set hints byte = 0, oder kann auch mit ZeroMemory nutzen
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    result = f_getaddrinfo(IP, PORT, serverSocket, result, hints);

    f_bind(PORT,serverSocket, result);

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