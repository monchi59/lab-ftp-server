CC=gcc
CFLAGS=-c -Wall -std=c99 -pedantic

all: ftp_server

ftp_server: ftp_server.o
	$(CC) $^ -o $@

%.o: %.c ftp_server.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ftp_server *.o
