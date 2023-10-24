client: client.c
	gcc client.c -o client

server: server.c
	gcc server.c -o server -lkeystone -g

docker: server.c
	cp server.c docker/server.c

all: client server

clear:
	rm server client