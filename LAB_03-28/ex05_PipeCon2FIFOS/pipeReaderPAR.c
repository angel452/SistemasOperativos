#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_BUF 1024

int main()
{
    int fdPAR;
    char * myfifoPAR = "/tmp/myfifoPAR";
    char buf[MAX_BUF];

    /* open, read, and display the message from the FIFO */
    fdPAR = open(myfifoPAR, O_RDONLY);
    read(fdPAR, buf, MAX_BUF);

    int num = atoi(buf); // Transformamos a entero lo que recivimos del buff

    num = num + 10;
    printf("Reveived %d", num);

    close(fdPAR);

    return 0;
}