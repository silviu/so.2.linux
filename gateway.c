#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <mqueue.h>

#define MQ_NAME "/gateway"

typedef struct command {
    char name[8];
    int dim;
    short crypt[17];
} Command, *PCommand;

mqd_t open_msg_queue()
{
    // unlink the message queue to be sure it is created
    mq_unlink(MQ_NAME);
    
    // initialize mq_attr stucture
	struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = 49;
    attr.mq_curmsgs = 0;

    
    // open message queue
    mqd_t ret = mq_open(MQ_NAME, O_CREAT | O_RDONLY, 0666, &attr);
    if (ret == (mqd_t)-1) {
        perror("mq_open");
        return (mqd_t)-1;
    }
    return ret;
}

PCommand get_message(mqd_t mq)
{
    unsigned int prio = 10;
    char* buff = (char*) malloc(100);
    mq_receive(mq, buff, 100, &prio);
    return (PCommand)buff;
}


int main(int argc, char** argv)
{
    printf("Gateway is up.\n");
    
    mqd_t mq = open_msg_queue();
    if (mq == (mqd_t)-1)
        exit(EXIT_FAILURE);
    PCommand c = get_message(mq);
    printf("Name = %s\nDim=%d\n", c->name, c->dim);
    
	
    return 0;
}
