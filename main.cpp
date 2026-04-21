#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cerrno>
#include <cstring>
#include <pthread.h>

#include "statistics.h"
#include "receiver_thread.h"

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


    // vars
    pthread_t thread;
    pthread_create(&thread, NULL, receiver_function, NULL);
    fstream outputFile;
    State state = IDLE;
    char raw[MAX_CAN_MESSAGE_SIZE];
    CanMessage message;
    int session_number = 0;
    map<uint16_t, MessageStats> statistics_map;

    while(1){

        // thread
        pthread_mutex_lock(&mut);
        while(!message_ready){
            pthread_cond_wait(&cond_var, &mut);
        }

        strcpy(raw, shared_message);
        message_ready = false;
        pthread_mutex_unlock(&mut);
        // cout << "Main: messaggio ricevuto " << raw << "\n";
        
        if(check_can_message(raw, &message) !=0){
            continue;
        }

        // FSM
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
                // CSV 
                save_csv(statistics_map, session_number);
                reset_stat(statistics_map);
            }else{
                // update a new data
                outputFile << "(" << time(NULL) << ") " << raw << "\n";
                //CSV
                update_stat(statistics_map, message.id);
            }
        }
    }

    pthread_join(thread, NULL);
    outputFile.close();
    close_can();
    return 0;
}