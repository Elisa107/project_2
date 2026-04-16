#include <stdio.h>
#include <iostream>
using namespace std;

#define CAN_FILE "../candump.log"

extern "C"{
    #include "fake_receiver.h"
    #include "can_message.h"
}

int main(void){

    if(open_can(CAN_FILE)!=0){
        printf("Errore apertura file\n");
        return -1;
    }

    State state = IDLE;
    char raw[MAX_CAN_MESSAGE_SIZE];
    CanMessage message;

    while(true){

        if(can_receive(raw) < 0){
            continue;
        }
        if(check_can_message(raw, &message) !=0){
            continue;
        }

        if(state == IDLE){
            if(check_start(&message)){
                //new session started
                state = RUN;
            }
        }else{
            if(check_stop(&message)){
                state = IDLE;
                // need to close the file
            }else{

                //save raw message in a file (each session has a different file)
                // the file needs a line for each message received file+timestamp
            }
        }
    }

    close_can();
    return 0;
}