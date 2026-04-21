# project_2 - basic part of a telemetry software

# Additional files:
# can_message.c
I added a struct to parse messages.
typedef struct {
    uint16_t id; 
    uint8_t  payload[MAX_PAYLOAD_SIZE];
    int      payload_bytes;
} CanMessage;

3 functions: 
int check_can_message(const char*, CanMessage*); 
It checks the id and payload of each message that has been read. It returns 0 if it's all good and -1 if there is a problem with the message structure.

int check_start(const CanMessage*); 
It checks if a message is a start message (return 1) or not (return 0).

int check_stop(const CanMessage*);
It checks if a message is a stop message (return 1) or not (return 0).

# statistics.cpp
I added a struct which contains the number of messages with the same ID, the mean_time of their arrival and the time that occurs from the last message arrived (always with the same ID).
I also used a map to avoid using arrays that need to be declared with a size, each map component contains the ID and the message's statistic.

3 functions:
void update_stat(map<uint16_t, MessageStats>&, uint16_t);
It calculates the mean_time in milliseconds between messages with the same ID.

void save_csv(map<uint16_t, MessageStats>&, int);
It creates a CSV file (in the directory "sessions") which contains: the message ID, the number of messages with the same ID and the mean_time.

void reset_stat(map<uint16_t, MessageStats>&);
It clears the map so for each session the program uses an empty map.

# receiver_thread.cpp
1 function:
void* receiver_function(void* arg);
It uses a thread to communicate with main.cpp. 
There's a shared variable which is the local_message that contains the message which has been read. This shared variable can be accessed from the receiver_thread or the main (not at the same time) thanks to the use of a mutex (which prevents data race between the two threads) and a condition variable.

# main.cpp
It manages the FSM. For each session (every time the state changes from IDLE to RUN) the main calls functions from other files to create new session_files (both text files with timestamp and message and CSVs with ID, number of messages and mean time).