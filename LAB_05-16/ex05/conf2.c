#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 0x1111

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
    // semget(llave, numero de semaforas que se quiere crear, permisos);
    int id = semget(KEY, 1, 0666 | IPC_CREAT);
    
    if (id < 0){
        perror("semget");
        exit(11);
    }

    // semop(id de la semafora, operacion, cantidad de operaciones a realizar)
    //lock...
    if (semop(id, &p, 1) < 0) // Operacion P
    {
        perror("semop p");
        exit(12);
    }

    for(int i = 0; i < 10; ){ // bucle infinito de imprimir "a"
        putchar('a');
        fflush(stdout);
    }

    if (semop(id, &v, 1) < 0) // Operacion V
    {
        perror("semop v");
        exit(13);
    }
}