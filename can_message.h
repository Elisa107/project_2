#ifndef CAN_MESSAGE_H
#define CAN_MESSAGE_H

#include <stdint.h>

#define MAX_PAYLOAD_SIZE 8

typedef struct {
    uint16_t id;
    uint8_t  payload[MAX_PAYLOAD_SIZE];
    int      payload_bytes;
} CanMessage;

typedef enum {
    IDLE,
    RUN
} State;

int check_can_message(const char* raw, CanMessage* message);
int check_start(const CanMessage* message);
int check_stop(const CanMessage* message);

#endif