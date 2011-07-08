#include "mq_common.h"

mqd_t open_msg_queue(char* mq_name)
{
    // initialize mq_attr stucture
	struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = sizeof(Command);
    attr.mq_curmsgs = 0;

    
    // open message queue
    mqd_t ret = mq_open(mq_name, O_CREAT | O_RDWR, 0666, &attr);
    if (ret == (mqd_t)-1) {
    	perror("mq_open!!!");
    	fprintf(stderr, "%s\n", mq_name);
        return (mqd_t)-1;
    }
    return ret;
}

int receive_command(mqd_t mq, Command *cmd)
{
    unsigned int prio = 10;
    struct mq_attr attr;
	mq_getattr(mq, &attr);

    int ret = mq_receive(mq, (char*) cmd, attr.mq_msgsize, &prio);
    if (ret == -1) {
    	perror("mq_receive");
    	return -1;
    }
    return 0;
}


int send_command(mqd_t mq, Command *cmd)
{
	unsigned int prio = 10;
	int ret = mq_send(mq, (const char*) cmd, sizeof(*cmd), prio);
	if (ret == -1) {
		perror("mq_send");
		return -1;
	}
	return 0;
}
