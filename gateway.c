#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <string.h>
#include "mq_common.h"

#define MQ_NAME "/gateway"
#define NUM_SERVERS 4
mqd_t server_mqs[NUM_SERVERS];

int open_server_mqs()
{
	int i;
	char mq_name[12];
	for (i = 0; i < NUM_SERVERS; i++) {
		sprintf(mq_name, "/mq_server_%d", i);
		server_mqs[i] = open_msg_queue(mq_name);
		if (server_mqs[i] == -1) {
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

int close_server_mqs()
{
	int i;
	for (i = 0; i < NUM_SERVERS; i++) {
		int ret = mq_close(server_mqs[i]);
		if (ret == -1) {
			perror("mq_close");
			return -1;
		}
	}
	return 0;		
}



int send_command(int server_id, PCommand command)
{
	unsigned int prio = 10;
	char* buff = (char*) command;
	int ret = mq_send(server_mqs[server_id], buff, strlen(buff), prio);
	if (ret == -1) {
		perror("mq_send");
		return -1;
	}
	return 0;
}

int unlock_client(char* sem_name)
{
	sem_t* client_sem = sem_open(sem_name, O_CREAT, 0644, 1);
	if (client_sem == SEM_FAILED) {
		perror("sem_open");
		return -1;
	}
	
	sem_post(client_sem);
	
	sem_close(client_sem);
	return 0;
}

int main(int argc, char** argv)
{
    printf("Gateway is up.\n");
    
    // open clients <-> gateway message queue
    mqd_t mq = open_msg_queue(MQ_NAME);
    if (mq == (mqd_t)-1)
        exit(EXIT_FAILURE);
    
    // open gateway <-> servers message queues
    open_server_mqs();
    
    PCommand command = receive_command(mq);
    int server_id = command->crypt[0];
    while(strcmp(command->name, "exit") != 0) {
    	printf("NAME=%s\n", command->name);
    	server_id = command->crypt[0];
    	// if there are no more actions to take
    	if (server_id == -1)
    		unlock_client(command->name);
    	else
    		send_command(server_id - 1, command);
    	
    	command = receive_command(mq);
    }
    // send the exit command to the servers
    send_command(server_id - 1, command);
    
    close_server_mqs();
    mq_close(mq);
    return 0;
}
