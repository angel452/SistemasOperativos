#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/shm.h>

#define SHMSZ     27 // Tamaño d e la share memory

int main(int argc, char **argv){

    int aux1, aux2;
    aux1 = atoi(argv[1]);
    aux2 = atoi(argv[2]);

    // ############ VARIABLES ##############
    char *shm, *s;

    // ######### ID ##############
    int shmid;
    key_t keySuma, keyResta, keyMultiplicacion, keyDivicion; // key
    keySuma = 101;
    keyResta = 102;
    keyMultiplicacion = 103;
    keyDivicion = 104;

    if ((shmid = shmget(keySuma, SHMSZ, 0666)) < 0) { 
        perror("shmget");
        exit(1);
    }
    else if ((shmid = shmget(keyResta, SHMSZ, 0666)) < 0) { 
        perror("shmget");
        exit(1);
    }
    else if ((shmid = shmget(keyMultiplicacion, SHMSZ, 0666)) < 0) { 
        perror("shmget");
        exit(1);
    }
    else if ((shmid = shmget(keyDivicion, SHMSZ, 0666)) < 0) { 
        perror("shmget");
        exit(1);
    }

    // USamos atach para recuperar 
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
    * Now read what the server put in the memory.
    */
    s = shm;
    putchar(*s);
    s++;
    putchar(*s);
    putchar('\n');

    kill(aux1, aux2);

    return 0;
}