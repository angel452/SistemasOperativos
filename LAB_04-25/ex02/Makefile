}CC = gcc
CFLAGS = -Wall

all: PO

PO: PO.c
	$(CC) $(CFLAGS) -o PO.exe PO.c
	$(CC) $(CFLAGS) -o P1.exe P1.c
	$(CC) $(CFLAGS) -o Ps.exe Ps.c

clean: 
	rm -f PO.exe
	rm -f P1.exe
	rm -f Ps.exe

run:
	./PO.exe
