#include "receiver_thread.h"
#include <pthread.h>
#include <iostream>
#include <string.h>

using namespace std;

extern "C"{
    #include "fake_receiver.h"
}

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
char shared_message[MAX_CAN_MESSAGE_SIZE];
bool message_ready = false;

void* receiver_function(void* arg){
    char local_message[MAX_CAN_MESSAGE_SIZE];

    while(1){
        if(can_receive(local_message) < 0){
            continue;
        }

        pthread_mutex_lock(&mut);
        strcpy(shared_message, local_message);
        message_ready = true;
        // cout << "Thread: messaggio inviato " << local_message << "\n";
        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&mut);
    }
    return NULL;
}