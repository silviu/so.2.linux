#include <sys/mman.h>
#include <unistd.h>
#include "mq_common.h"
#include "crypto.h"

#define MQ_GATEWAY "/gateway"

typedef int (*pdecryptf_t) (void*, int, void*);
pdecryptf_t v[4] = {decrypt_base64, decrypt_des, decrypt_bf, decrypt_rc2};

pdecryptf_t choose_decryption(int server_id)
{
	return v[server_id-1];
}

static void* open_shared_memory(char* shm_name, int shm_size, int* shm_fd)
{	
	*shm_fd = shm_open(shm_name, O_RDWR, 0666);
	if (*shm_fd == -1) {
		perror("shm_open");
		return NULL;
	}
	
	void* mem = mmap(0, shm_size, PROT_WRITE | PROT_READ, 
					 MAP_SHARED, *shm_fd, 0);
	if (mem == MAP_FAILED) {
		int rc = close(*shm_fd);
		if (rc == -1) {
			perror("close");
			return NULL;
		}
		perror("mmap");
		return NULL;
	}
	
	return mem;
}

static int close_shared_memory(void* mem, int shm_size, int shm_fd, char* shm_name)
{
	/* unmap shm */
	int rc = munmap(mem, shm_size);
	if (rc == -1) {
		perror("munmap");
		return -1;
	}
 
	/* close descriptor */
	rc = close(shm_fd);
	if (rc == -1) {
		perror("close");
		return -1;
	}

	return 0;
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		fprintf(stderr, "Too few arguments given to server.\n./server N\n");
		exit(EXIT_FAILURE);
	}
	
	int server_id = atoi(argv[1]);
	pdecryptf_t decrypt = choose_decryption(server_id);
	
	char mq_name[13];
	sprintf(mq_name, "/mq_server_%d", server_id);
	mqd_t mq = open_msg_queue(mq_name);
	
	Command cmd;
    int ret = receive_command(mq, &cmd);
    if (ret == -1)
    	exit(EXIT_FAILURE);
    Command *command = &cmd;
	
	mqd_t ret_mq;
	while(strcmp(command->name, "exit") != 0) {
		
		//open shared memory
		char* shm_name = command->name;
		int shm_size = command->dim;
		int shm_fd;
		void* mem = open_shared_memory(shm_name, shm_size, &shm_fd);
		if (mem == NULL) {
			exit(EXIT_FAILURE);
		}
		
		// get size of the message from the first 
		// integer in the shared memory
		int dim_message;
		memcpy(&dim_message, mem, sizeof(int));
		
		// decrypt message
		void* out = malloc(1000);
		int decrypt_size = decrypt(mem+sizeof(int), dim_message, out);
		
		// copy result back in shared memory
		memcpy(mem, &decrypt_size, sizeof(int));
		memcpy(mem + sizeof(int), out, decrypt_size);
		
		// Close shared memory
		close_shared_memory(mem, shm_size, shm_fd, shm_name);
		
		// Send message back to gateway with self removed
		int i;
		for (i = 1; i < 17; i++)
			command->crypt[i-1] = command->crypt[i]; 
		ret_mq = open_msg_queue(MQ_GATEWAY);
		ret = send_command(ret_mq, command);
		if (ret == -1)
    		exit(EXIT_FAILURE);
		mq_close(ret_mq);
		
		// Get another message from the mq
		ret = receive_command(mq, command);
		if (ret == -1)
    		exit(EXIT_FAILURE);
	}
	mq_close(mq);
	mq_close(ret_mq);
    return 0;
}
