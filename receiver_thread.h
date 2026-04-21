#ifndef RECEIVER_THREAD_H
#define RECEIVER_THREAD_H

#include <pthread.h>
#include <stdbool.h>

extern "C"{
    #include "fake_receiver.h"
}

extern pthread_mutex_t mut;
extern pthread_cond_t cond_var;
extern char shared_message[MAX_CAN_MESSAGE_SIZE];
extern bool message_ready;

void* receiver_function(void* arg);

#endif