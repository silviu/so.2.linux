#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "mq_common.h"

#define MQ_NAME "/gateway"
#define NUM_SERVERS 4
mqd_t server_mqs[NUM_SERVERS];

static int create_server_mqs()
{
	int i;
	char mq_name[12];
	for (i = 0; i < NUM_SERVERS; i++) {
		sprintf(mq_name, "/mq_server_%d", i+1);
		server_mqs[i] = create_msg_queue(mq_name);
		printf("Opened %s\n", mq_name);
		if (server_mqs[i] == (mqd_t)-1) {
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

static int close_server_mqs()
{
	int i;
	for (i = 0; i < NUM_SERVERS; i++) {
		int ret = mq_close(server_mqs[i]);
		if (ret == -1) {
			perror("mq_close");
			return -1;
		}
		char mq_name[12];
		sprintf(mq_name, "/mq_server_%d", i+1);
		ret = mq_unlink(mq_name);
		if (ret == -1) {
			perror("mq_unlink");
			return -1;
		}
	}
	return 0;		
}


static int unlock_client(char* sem_name)
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
    mqd_t mq = create_msg_queue(MQ_NAME);
    if (mq == (mqd_t)-1)
        exit(EXIT_FAILURE);
    
    // open gateway <-> servers message queues
    create_server_mqs();
    Command cmd;
    int ret = receive_command(mq, &cmd);
    if (ret == -1)
    	exit(EXIT_FAILURE);
    Command *command = &cmd;
    int server_id = command->crypt[0];
    
    while(strcmp(command->name, "exit") != 0) {
    	printf("NAME = %s\n", command->name);
    	server_id = command->crypt[0] - 1;
    	// if there are no more actions to take
    	if (server_id == -2) {
    		printf("UNLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOK!\n\n");
    		unlock_client(command->name);
    	}
    	else {
    		send_command(server_mqs[server_id], command);
    		printf("SENT COMMAND to server_id = %d\n", server_id);
    	}
    	
    	ret = receive_command(mq, command);
    	if (ret == -1)
    		exit(EXIT_FAILURE);
    }
    // send the exit command to the servers
    int i;
    for (i = 0; i < NUM_SERVERS; i++)
    	send_command(server_mqs[i], command);
    
    close_server_mqs();
    mq_close(mq);
    //mq_unlink(MQ_NAME);
    return 0;
}
