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

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

int main()
{
    // ########### SHARE MEMORY #####################
    // --> Variables
    int shmid;
    key_t key_shm = 5678;
    char *s, *shm;

    // -->  Localizamos el share memory
    if ( (shmid = shmget(key_shm, SHMSZ, 0666)) < 0) {
        perror("shmget loweCase");
        exit(1);
    }

    // --> Attach 
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // ---------------------------------------------------------------------

    // ########### OBTENER LAS SEMAFORAS #############
    int id = semget(KEY, 1, 0666 | IPC_CREAT);
    // --> Verificar error
    if (id < 0)
    {
        perror("semget");
        exit(11);
    }

    // ############## LOCK SEMAFORA ##################
    // --> Variables
    char *cadena = "abcdefghi";
    int l = strlen(cadena);

    // --> Filtro de tipo de letra
    for (int i = 0; i < l; ++i){
        // --> Lock
        if(semop(id, &p, 1) < 0){
            perror("sempop p");
            exit(12);
        }

        // --> Reed share memory
        s = shm;

        // --> Pregunta si toca una minuscula
        //printf("\n");
        //printf("In LowerCase [%d]... Valor del share memory: ", i);
        //putchar(*s);
        //printf("\n");

        if(*s == 'L'){
            putchar(cadena[i]);
            fflush(stdout);

            putchar(cadena[i]);
            fflush(stdout);

            *s = 'U'; //El siguiente sera las mayusculas 
            sleep(1);

            // --> Soltamos lock
            if (semop(id, &v, 1) < 0)
            {
                perror("semop v");
                exit(14);
            }
        }
        else{
            // --> Soltamos lock
            if (semop(id, &v, 1) < 0)
            {
                perror("semop v");
                exit(14);
            }
        }
    }
}