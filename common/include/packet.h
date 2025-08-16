#ifndef PACKET_H
#define PACKET_H

#define MAX_SNUM_LENGTH 10
#define MAX_TEXT_LENGTH 1024

typedef struct {
    char sNum[MAX_SNUM_LENGTH];
    char text[MAX_TEXT_LENGTH];
} packet;

#endif // PACKET_H