#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct command {
    char name[8];
    int dim;
    short crypt[17];
} Command, *PCommand;


mqd_t open_msg_queue(char* mq_name);
PCommand receive_command(mqd_t mq);
