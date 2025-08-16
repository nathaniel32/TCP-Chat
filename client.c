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

SOCKET f_clientSocket(struct addrinfo* result){
    //ein TCP Socket bauen
    SOCKET fclientSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (fclientSocket == INVALID_SOCKET) {
        perror("Client socket creation failed");
        freeaddrinfo(result);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Client socket created successfully...\n");
    return fclientSocket;
}

void f_connToServer(SOCKET clientSocket, struct addrinfo* result){
    if (connect(clientSocket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR) {
        //Die Connect-Funktion stellt eine Verbindung mit einem angegebenen Socket her.
        
        fprintf(stderr, "Connection to server failed: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        freeaddrinfo(result);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Connected to server successfully...\n");
}

struct addrinfo* f_getaddrinfo(const char* IP,const char* PORT, struct addrinfo hints, struct addrinfo* result){
    //Adressinformationen von Gastgebern und Diensten zu erhalten
    //Ergebnis wird in result speichert
    
    if (getaddrinfo(IP, PORT, &hints, &result) != 0) {
        perror("getaddrinfo failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return result;
}

void f_delete(int index){
    for(int a=1; a <= index+5; a++){
        printf("\b \b");
    }
}

void f_chat(SOCKET clientSocket, char myid[]){
    fd_set read_fds;
    struct timeval timeout;
    char message[MAX_MESSAGE_LENGTH];
    char buf[MAX_BUFFER_LENGTH];

    int index = -1;
    char ch;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(clientSocket, &read_fds);

        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int ready_r = select(0, &read_fds, NULL, NULL, &timeout); //non blocking read

        if (ready_r == SOCKET_ERROR) {
            perror("select");
            break;
        }

        if (ready_r > 0 && FD_ISSET(clientSocket, &read_fds)) { //Überprüfung der Verfügbarkeit der zu lesenden Daten.
            packet myPacket;

            int bytesReceived = recv(clientSocket, (char*)&myPacket, sizeof(myPacket), 0);

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
            } else if (bytesReceived == 0) {
                printf("Connection closed by the server\n");
                break;
            } else {
                //perror("recv");
                printf("Connection interrupted by the server\n");
                break;
            }
        }

        if (_kbhit()) { //wenn Benutzer etwas eingibt
            ch = _getch();
            if (ch == 8 && index > 0) {
                printf("\b \b");
                index--;
            } else if (ch == 8 && index == 0) {
                index = 0;
            } else {
                if(index == -1){
                    printf("You: ");
                    index = 0;
                }
                message[index++] = ch;
                printf("%c", ch);
            }

            if (ch == '\r' || ch == '\n' || index >= sizeof(message) - 1) {
                message[index] = '\0';

                if(strcmp(message, "Quit\r") == 0){
                    break;
                }

                packet myPacket;

                strcpy(myPacket.sNum, myid); //snum speichern im paket
                myPacket.sNum[sizeof(myPacket.sNum) - 1] = '\0';

                strcpy(myPacket.text, message);
                myPacket.text[sizeof(myPacket.text) - 1] = '\0';

                send(clientSocket, (void*)&myPacket, sizeof(myPacket), 0);

                printf("\n");
                index = -1;
            }
        }
    }
}

void f_close(SOCKET clientSocket){
    printf("Closing Connection...\n");
    closesocket(clientSocket); //Die Closesocket-Funktion schließt einen vorhandenen Socket.
    WSACleanup();
}

int main(int argc,char* argv[]) {

    if(argc < 4){
        printf("Usage: %s <ipv4- Adresse> <Port> <S-Nummer>",argv[0]);
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

    //Die addrinfo-Struktur wird von der getaddrinfo-Funktion verwendet, um Hostadresseninformationen zu enthalten.    

    SOCKET clientSocket;

    memset(&hints, 0, sizeof(hints));  //set hints byte = 0, oder kann auch mit ZeroMemory nutzen.
    hints.ai_family = AF_INET;
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