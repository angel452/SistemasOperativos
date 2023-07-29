#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_BUF 1024

int main()
{
    int fdIMPAR;
    char * myfifoIMPAR = "/tmp/myfifoIMPAR";
    char buf[MAX_BUF];

    /* open, read, and display the message from the FIFO */
    fdIMPAR = open(myfifoIMPAR, O_RDONLY);
    read(fdIMPAR, buf, MAX_BUF);

    int num = atoi(buf); // Transformamos a entero lo que recivimos del buff

    num = num + 10;
    printf("Reveived %d", num);


    close(fdIMPAR);

    return 0;
}