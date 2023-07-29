#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_BUF 1024

int main()
{
    int fd;
    char * myfifo = "/tmp/myfifo";
    char buf[MAX_BUF];

    /* open, read, and display the message from the FIFO */
    fd = open(myfifo, O_RDONLY);
    read(fd, buf, MAX_BUF);

    int num = atoi(buf); // Transformamos a entero lo que recivimos del buff
    num = num + 10;
    
    printf("Reveived %d", num); // Imprimimos
    close(fd);

    return 0;
}