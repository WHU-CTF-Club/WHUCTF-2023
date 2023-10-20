client: client.c
	gcc client.c -o client

server: server.c
	gcc server.c -o server -lkeystone -g

all: client server

clear:
	rm server client