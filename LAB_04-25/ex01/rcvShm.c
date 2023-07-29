/*
 * shm-client - client program to demonstrate shared memory.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ     27 // Tamaño d e la share memory

main()
{
    int shmid; // id del shm
    key_t key; // llave
    char *shm, *s;

    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 5678;

    /*
     * Locate the segment.
     */
    // a diferencia del otro programa, este ya no llama al IPC_CREAT, pues ya esta creado
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) { 
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    // USamos atach para recuperar 
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * Now read what the server put in the memory.
     */
    for (s = shm; *s != NULL; s++)
        putchar(*s);
    putchar('\n');

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shm = '*'; // ponemos un asterisco en la primera posicion para que pare el otro programa

    exit(0);
}