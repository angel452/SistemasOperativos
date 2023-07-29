}CC = gcc
CFLAGS = -Wall

all: recibirSignal

recibirSignal: recibirSignal.c
	$(CC) $(CFLAGS) -o recibirSignal recibirSignal.c
	$(CC) $(CFLAGS) -o enviarSignal enviarSignal.c

clean: 
	rm -f recibirSignal
	rm -f enviarSignal

run:
	./recibirSignal
