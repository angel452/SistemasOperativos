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

    if (id < 0)
    {
        perror("semget");
        exit(11);
    }

    union semun u;
    u.val = 3;
    
    // semctl(id de la semafora, indice de la semafora, que hacer con la semafora?, seteamos la estructura u a la semafora)
    if (semctl(id, 0, SETVAL, u) < 0)
    {
        perror("semctl");
        exit(12);
    }
}