#include "statistics.h"
#include <fstream>
#include <iostream>

using namespace std;

void update_stat(map<uint16_t, MessageStats>& statistics_map, uint16_t id){
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    if(statistics_map.find(id) == statistics_map.end()){
        statistics_map[id].num_messages = 1;
        statistics_map[id].mean_time = 0;
        statistics_map[id].last_time = now;
    }else{
        double elapsed_time = (now.tv_sec - statistics_map[id].last_time.tv_sec) * 1000.0 + (now.tv_nsec - statistics_map[id].last_time.tv_nsec) / 1000000.0;
        statistics_map[id].num_messages++;
        statistics_map[id].mean_time += (elapsed_time - statistics_map[id].mean_time) / statistics_map[id].num_messages;
        statistics_map[id].last_time = now;
    }
}

void save_csv(map<uint16_t, MessageStats>& statistics_map, int session_number){
    fstream csv;
    csv.open("../sessions/statistics_" + to_string(session_number) + ".csv", ios::out);
    if(csv.fail()){
        cout << "Errore apertura file CSV\n";
        return;
    }

    csv << "ID; number_of_messages; mean_time\n";
    for(auto& i : statistics_map){
        csv << i.first << ";" << i.second.num_messages << ";" << i.second.mean_time << "\n";
    }
    csv.close();
}


void reset_stat(map<uint16_t, MessageStats>& statistics_map){
    statistics_map.clear();
}