CC = gcc
CFLAGS = -Wall -g -lrt
 
build: all

all:clean server gateway

server: server.c
	@echo "Compiling sources..."
	$(CC) $(CFLAGS) -L. -lcrypto server.c mq_common.c -o server

gateway: gateway.c
	$(CC) $(CFLAGS) gateway.c mq_common.c -o gateway
	@echo "------------------------------"

test: clean server gateway
	cd checker; make -f Makefile.checker
	@echo "------------------------------"

clean:
	@echo "Cleaning workspace..."
	rm -rf *.o *~ gateway server
	@echo "------------------------------"
