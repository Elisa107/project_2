#include "can_message.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int check_can_message(const char* raw, CanMessage* message){
    const char* separator = strchr(raw, '#');
    if (separator==NULL){
        return -1;
    }

    // controllo ID
    int id_len = separator;
    if (id_len == 0 || id_len > 3){
        return -1;
    }

    char id_str[4] = {0};
    strncpy(id_str, raw, id_len);
    message->id = (uint16_t)strtol(id_str, NULL, 16); // converto stringa in intero esadecimale 

    // controllo payload
    char* payload_str = separator +1;
    int payload_char = strlen(payload_str);
    if (payload_char == 0 || payload_char%2 != 0 || payload_char > 16){
        return -1;
    }
    message->payload_bytes = payload_char/2;

    for (int i=0; i<message->payload_bytes i++){
        char byte[3] = {0};
        int index = i*2;
        byte[0] = payload_str[index];
        byte[1] = payload_str[index+1];
        message->payload[i] = (uint8_t)strtol(byte, NULL, 16);
    }
    return 0;
}

int check_start_message(const CanMessage* message){
    if (message->id != 0x0A0)
        return 0;
    if (msg->payload_len != 2)
        return 0;
    if (msg->payload[0] == 0x66 && msg->payload[1] == 0x01)
        return 1;
    if (msg->payload[0] == 0xFF && msg->payload[1] == 0x01)
        return 1;
    return 0;
}

int check_stop_message(const CanMessage* message){
    if (message->id != 0x0A0)
        return 0;
    if (message->payload_bytes != 2)
        return 0;
    if (message->payload[0] == 0x66 && message->payload[1] == 0xFF)
        return 1;
    return 0;
}