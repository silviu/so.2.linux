#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct command {
    char name[8];
    int dim;
    short crypt[17];
} Command, *PCommand;

mqd_t open_msg_queue(char* mq_name);
mqd_t create_msg_queue(char* mq_name);
int receive_command(mqd_t mq, Command *cmd);
int send_command(mqd_t mq, PCommand command);
