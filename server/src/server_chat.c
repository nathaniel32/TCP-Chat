#include "../include/server.h"

void f_delete(int index) {
    // delete text
    for(int a = 1; a <= index + 5; a++) {
        printf("\b \b");
    }
}

void f_chat(SOCKET newSocket, char myid[]) {
    char message[MAX_MESSAGE_LENGTH];
    char buf[MAX_BUFFER_LENGTH];

    // Parameter für select()
    fd_set read_fds;
    struct timeval timeout;

    int index = -1;
    char ch;

    while (1) {
        FD_ZERO(&read_fds); // Initialisiert den Satz auf den leeren Satz. Ein Set sollte vor der Verwendung immer gelöscht werden.
        FD_SET(newSocket, &read_fds); // Fügt Socket zum read_fds hinzu. Man kann mehrere Socket in read_fds speichern.

        timeout.tv_sec = 0; // seconds
        timeout.tv_usec = 0; // microseconds

        int ready_r = select(0, &read_fds, NULL, NULL, &timeout); // non blocking read

        if (ready_r == SOCKET_ERROR) {
            perror("select");
            break;
        }

        if (ready_r > 0 && FD_ISSET(newSocket, &read_fds)) { // Überprüfung der Verfügbarkeit der zu lesenden Daten.
            packet myPacket;

            int bytesReceived = recv(newSocket, (char*)&myPacket, sizeof(myPacket), 0);

            // flags (letzte Spalten) : Bitflags, die das Verhalten des Recv-Aufrufs steuern, wie MSG_PEEK,MSG_WAITALL, usw.

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
            } else if (bytesReceived == 0) { // wenn Client f_close() gerufen hätte.
                printf("Connection closed by the client\n");
                break;
            } else {
                printf("Connection interrupted by the client\n");
                break;
            }
        }

        if (_kbhit()) { // wenn Benutzer etwas eingibt
            ch = _getch();
            if (ch == 8 && index > 0) {
                printf("\b \b"); // delete
                index--;
            } else if (ch == 8 && index == 0) {
                index = 0; // passiert nix da kein mehr message zu löschen
            } else {
                if(index == -1) {
                    printf("You: ");
                    index = 0;
                }
                message[index++] = ch; // laenge von message zunehmend
                printf("%c", ch);
            }

            if (ch == '\r' || ch == '\n' || index >= sizeof(message) - 1) {
                message[index] = '\0'; // als string wechseln

                if(strcmp(message, "Quit\r") == 0) {
                    break;
                }

                packet myPacket;
                strcpy(myPacket.sNum, myid); // snum speichern im paket
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