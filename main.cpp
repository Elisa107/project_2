#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cerrno>
#include <cstring>

using namespace std;

#define CAN_FILE "../candump.log"

extern "C"{
    #include "fake_receiver.h"
    #include "can_message.h"
}

int main(void){

    if(open_can(CAN_FILE)!=0){
        cout << "Errore apertura file\n";
        return -1;
    }

    fstream outputFile;
    State state = IDLE;
    char raw[MAX_CAN_MESSAGE_SIZE];
    CanMessage message;
    int session_number = 0;

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
                if(outputFile.is_open()){
                    outputFile.close();
                }

                session_number++;
                outputFile.open("../sessions/session_" + to_string(session_number) + ".log", ios::out);

                if(outputFile.fail()){
                    cout << "Errore apertura file sessione: " << strerror(errno) << "\n";
                    return -1;
                }

                state = RUN;
            }
        }else{
            if(check_stop(&message)){
                // need to close the file, session finished
                state = IDLE;
                if(outputFile.is_open()){
                    outputFile.close();
                }
            }else{
                outputFile << "(" << time(NULL) << ") " << raw << "\n";
            }
        }
    }

    outputFile.close();
    close_can();
    return 0;
}