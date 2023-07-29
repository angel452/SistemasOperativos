/*****************************************************************************
 Excerpt from "Linux Programmer's Guide - Chapter 6"
 (C)opyright 1994-1995, Scott Burkett
 ***************************************************************************** 
 MODULE: pipe.c
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>


int c;


int main(void)
{
        int fdp[2]; 
        // ########################################################
        //fdp[0] = read / input
        //        Cerramos lectura pues vamos a escribir
        //        close(fdp[0]);
        //        write(fdp[1], string, (strlen(string)+1));

        //fdp[1] = write / output 
        //       Cerramos lectura pues vamos a leer
        //       close(fdp[1]);
        //       read(fdp[0], readbuffer, sizeof(readbuffer));
        // ########################################################

        int nbytes;
        pid_t   childpid;
        char    string[] = "Hello, world!\n";
        char    readbuffer[80];
        int c;

        pipe(fdp);

        childpid = fork();

        printf("Main program: \n");

        if(childpid == 0)
        {
                int c;
                printf("We are in the child process: \n"); 
                //printf("Send somethig to your father: ");
                c = getchar();
                printf("From Child: %c\n",c);

                /* Child process closes up input - read side of pipe */
                close(fdp[0]);

                /* Send "string" through the output side of pipe */
                write(fdp[1], string, (strlen(string)+1));
                exit(0);
        }
        else
        {
                int c;
                printf("We are in the parent process: \n"); 
                //printf("Send somethig to your child: ");
                c = getchar();
                printf("From Parent: %c\n",c);
                /* Parent process closes up output side of pipe */
                close(fdp[1]);

                /* Read in a string from the pipe */
                nbytes = read(fdp[0], readbuffer, sizeof(readbuffer));
                printf("Received string: %s", readbuffer);
        }

        return(0);
}
