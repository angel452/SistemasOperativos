}CC = gcc
CFLAGS = -Wall

all: main

main: conf.c
	$(CC) $(CFLAGS) -o conf.exe conf.c
	$(CC) $(CFLAGS) -o lowerCase.exe lowerCase.c
	$(CC) $(CFLAGS) -o upperCase.exe upperCase.c
	$(CC) $(CFLAGS) -o numberCase.exe numberCase.c

clean:
	rm -f conf.exe
	rm -f lowerCase.exe
	rm -f upperCase.exe
	rm -f numberCase.exe

run:
	./conf.exe & ./lowerCase.exe & ./upperCase.exe & ./numberCase.exe