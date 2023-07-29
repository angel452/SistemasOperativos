#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#define MAXSIZE     128
#define SHMSZ     27 // Tamaño de la shm (share memory)

// ################## FUNCIONES PARA EL IPCS ##############
void die(char *s)
{
    perror(s);
    exit(1);
}

struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};


void main()
{

    // ####################### RECIVIR UN NUMERO ####################
    // ######### ID ##############
    int msqid1; // id del msq

    key_t key1; // key
    key1 = 1234;

    // ############ VARIABLES ##############
    struct msgbuf rcvSbuf1;

    // ############ VERIFICADOR DE ERROR ################
    if ((msqid1 = msgget(key1, 0666)) < 0)
        die("msgget()");

    // ############ VERIFICADOR DE ERROR ################
    if (msgrcv(msqid1, &rcvSbuf1, MAXSIZE, 1, 0) < 0) // FALLO AL RECIBIR 
        die("msgrcv");

    int auxNum = atoi(rcvSbuf1.mtext);
    auxNum = auxNum + 1234;

    printf("Res Prg2: %s, - , %d \n", rcvSbuf1.mtext, auxNum); // SI SE RECIVIO EL MENSAJE

    //exit(0);

    // ####################### ENVIAR  UN NUMERO ####################
    char c;
    int shmid;
    key_t key;
    char *shm, *s;

    /*
    * We'll name our shared memory segment
    * "5678".
    */
    key = 5678;

    /*
    * Create the segment.
    */
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
    * Now we attach the segment to our data space.
    */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
    * Now put some things into the memory for the
    * other process to read.
    */
    s = shm;

    for (c = 'a'; c <= 'z'; c++)
        *s++ = c; 
        // en el contenido de s, colocar la a. Luego sumar el puntero ++
        // luego 
    *s = NULL;

    /*
    * Finally, we wait until the other process 
    * changes the first character of our memory
    * to '*', indicating that it has read what 
    * we put there.
    */
    while (*shm != '*')
        sleep(1);

    exit(0);
}

