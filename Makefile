CC=gcc
CFLAGS=-c -Wall -std=c99 -pedantic

all: ftp_server

ftp_server: ftp_server.o read_line.o cmd_handler.o log.O
	$(CC) $^ -o $@

%.o: %.c ftp_server.h read_line.h cmd_handler.h log.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ftp_server *.o
