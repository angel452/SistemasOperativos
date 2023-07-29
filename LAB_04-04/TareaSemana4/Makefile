}CC = gcc
CFLAGS = -Wall

all:  process1

process1: process1.c	
	$(CC) $(CFLAGS) -o process1.exe process1.c
	$(CC) $(CFLAGS) -o readerProcess2.exe readerProcess2.c


clean: 
	rm -f readerProcess2.exe
	rm -f process1.exe

run:
	./process1.exe
