#ifndef PACKET_H
#define PACKET_H

#define MAX_SNUM_LENGTH 10
#define MAX_TEXT_LENGTH 1024

typedef struct {
    char sNum[MAX_SNUM_LENGTH];
    char text[MAX_TEXT_LENGTH];
} packet;

#define MAX_MESSAGE_LENGTH 1024
#define MAX_BUFFER_LENGTH 1024

#endif // PACKET_H