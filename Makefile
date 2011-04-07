CC = gcc
CFLAGS = -Wall -g -lrt

all:clean server gateway

server: server.c
	$(CC) $(CFLAGS) server.c -o server

gateway: gateway.c
	$(CC) $(CFLAGS) gateway.c -o gateway

clean:
	rm -rf *.o *~ gateway server
