#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include <sys/sem.h>

#include <sys/shm.h>

#define KEY     0x1111
#define SHMSZ   27

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

int main(){
    // ########### SHARE MEMORY #####################
    // --> Variables
    int shmid;
    key_t key_shm = 5678;
    char *s, *shm;

    // -->  Crear share memory
    if ( (shmid = shmget(key_shm, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget conf");
        exit(1);
    }

    // --> Attach 
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // --> Dar valores
    s = shm; 
    *s = 'L'; // iniciamos con minusculas
    //s++; // aumentamos una posicion
    //*s = NULL; // Ponemos al final un null

    // ---------------------------------------------------------------------

    // ########### OBTENER LAS SEMAFORAS #############
    int id = semget(KEY, 1, 0666 | IPC_CREAT);
    // --> Verificar error
    if(id < 0){
        perror("semget");
        exit(11);
    }

    // ######### CANTIDAD DE RECURSOS ###############
    union semun recursos;
    recursos.val = 1;

    // ############# CONTROLADOR ###################
    if(semctl(id, 0, SETVAL, recursos) < 0){
        perror("semctl");
        exit(12);
    }
}