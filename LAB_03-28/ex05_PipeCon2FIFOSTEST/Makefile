}CC = gcc
CFLAGS = -Wall

all: pipeWriter

pipeWriter: pipeWriter.c
	$(CC) $(CFLAGS) -o pipeWriter pipeWriter.c
	$(CC) $(CFLAGS) -o pipeReaderPAR pipeReaderPAR.c
	$(CC) $(CFLAGS) -o pipeReaderIMPAR pipeReaderIMPAR.c

clean: 
	rm -f pipeWriter
	rm -f pipeReaderPAR
	rm -f pipeReaderIMPAR

run:
	./pipeWriter 50 + 21
