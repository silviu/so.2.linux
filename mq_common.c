#include "mq_common.h"

mqd_t open_msg_queue(char* mq_name)
{
    // unlink the message queue to be sure it is created
    mq_unlink(mq_name);
    
    // initialize mq_attr stucture
	struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = 49;
    attr.mq_curmsgs = 0;

    
    // open message queue
    mqd_t ret = mq_open(mq_name, O_CREAT | O_RDWR, 0666, &attr);
    if (ret == (mqd_t)-1) {
    	fprintf(stderr, "%s\n", mq_name);
        perror("mq_open!!!");
        return (mqd_t)-1;
    }
    return ret;
}

PCommand receive_command(mqd_t mq)
{
    unsigned int prio = 10;
    char* buff = (char*) malloc(100);
    int ret = mq_receive(mq, buff, 100, &prio);
    if (ret == -1) {
    	perror("mq_receive");
    	return NULL;
    }
    return (PCommand)buff;
}


int send_command(mqd_t mq, PCommand command)
{
	unsigned int prio = 10;
	char* buff = (char*) command;
	int ret = mq_send(mq, buff, strlen(buff), prio);
	if (ret == -1) {
		perror("mq_send");
		return -1;
	}
	return 0;
}
