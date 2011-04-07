#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <mqueue.h>

#define MQ_NAME "/gateway"

mqd_t open_msg_queue()
{
    //unlink the message queue to be sure it is created
    mq_unlink(MQ_NAME);

    mqd_t ret = mq_open(MQ_NAME, O_CREAT | O_RDONLY, 0666, NULL);
    if (ret == (mqd_t)-1) {
        fprintf(stderr, "Error on mq_open() in open_msg_queue()\n");
        perror("bls");
        return (mqd_t)-1;
    }

    return ret;
}

int main(int argc, char** argv)
{
    printf("Gateway is up.\n");
    open_msg_queue();
    return 0;
}
