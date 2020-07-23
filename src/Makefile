CFLAGS = -Wall -g
CC = cc
SOURCELIST = list.c server_help.c helpers.c

build_server:
	$(CC) $(CFLAGS) $(SOURCELIST) server.c -o server
build_client:
	$(CC) $(CFLAGS) helpers.c client.c -o client
run_server:
	./server
run_client:
	./client

clean:
	rm -f server client
