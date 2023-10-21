build: checkin.c
	gcc -O2 -static checkin.c -o checkin
run: checkin
	./checkin
docker: build
	cp checkin docker/checkin
clear: checkin
	rm checkin > /dev/null