#ifndef STATISTICS_H
#define STATISTICS_H

#include <map>
#include <cstdint>
#include <ctime>

using namespace std;

typedef struct {
    int num_messages;
    double mean_time;
    struct timespec last_time; // millisecondi
} MessageStats;

/*
struct timespec {
    time_t tv_sec;
    long tv_nsec;
};
*/

void update_stat(map<uint16_t, MessageStats>& statistics_map, uint16_t id);
void save_csv(map<uint16_t, MessageStats>& statistics_map, int session_number);
void reset_stat(map<uint16_t, MessageStats>& statistics_map);

#endif