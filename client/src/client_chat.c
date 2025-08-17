#include "../include/client.h"
#include <conio.h> //kbhit

void f_chat(SOCKET clientSocket, char myid[]) {
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

        int ready_r = select(0, &read_fds, NULL, NULL, &timeout); // non blocking read

        if (ready_r == SOCKET_ERROR) {
            perror("select");
            break;
        }

        if (ready_r > 0 && FD_ISSET(clientSocket, &read_fds)) { // Überprüfung der Verfügbarkeit der zu lesenden Daten.
            packet myPacket;

            int bytesReceived = recv(clientSocket, (char*)&myPacket, sizeof(myPacket), 0);

            if (bytesReceived > 0) {
                if (index >= 0) {
                    message[index] = '\0';
                    // printf("\r");
                    f_delete(index);
                    printf("%s: %s\n", myPacket.sNum, myPacket.text);
                    printf("You: %s", message);
                } else {
                    printf("%s: %s\n", myPacket.sNum, myPacket.text);
                }
            } else if (bytesReceived == 0) {
                printf("Connection closed by the server\n");
                break;
            } else {
                // perror("recv");
                printf("Connection interrupted by the server\n");
                break;
            }
        }

        if (_kbhit()) { // wenn Benutzer etwas eingibt
            ch = _getch();
            if (ch == 8 && index > 0) {
                printf("\b \b");
                index--;
            } else if (ch == 8 && index == 0) {
                index = 0;
            } else {
                if(index == -1) {
                    printf("You: ");
                    index = 0;
                }
                message[index++] = ch;
                printf("%c", ch);
            }

            if (ch == '\r' || ch == '\n' || index >= sizeof(message) - 1) {
                message[index] = '\0';

                if(strcmp(message, "Quit\r") == 0) {
                    break;
                }

                packet myPacket;

                strcpy(myPacket.sNum, myid); // snum speichern im paket
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