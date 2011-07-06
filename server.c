#include "mq_common.h"

int main(int argc, char** argv)
{
	if (argc < 2) {
		fprintf(stderr, "Too few arguments given to server.(./server N)");
		exit(EXIT_FAILURE);
	}
	int server_type = atoi(argv[1]);
	printf("%d\n", server_type);
    printf("Server is up.\n");
    return 0;
}
