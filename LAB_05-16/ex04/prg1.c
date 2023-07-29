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
    int id = semget(KEY, 1, 0666 | IPC_CREAT);

    if (id < 0) // error
    {
        perror("semget");
        exit(11);
    }

    union semun u;
    u.val = 1;
    
    if (semctl(id, 0, SETVAL, u) < 0)
    {
        perror("semctl");
        exit(12);
    }

    int pid;
    srand(pid);
    //printf("El numero aleatorio es: %d\n", pid);
    if (pid < 0)
    {
        printf("error");
        perror("fork");
        exit(1);
    }

    char *s = "abcdefgh";
    int l = strlen(s);

    for (int i = 0; i < l; ++i)
    {
        if (semop(id, &p, 1) < 0) // lanzo la operacion p. Si da negativo... error
        {
            perror("semop p");
            exit(13);
        }

        putchar(s[i]);
        fflush(stdout); // Forzar a imprimir en consola y limpiar el buffer

        sleep(rand() % 2);
        
        putchar(s[i]);
        fflush(stdout); // Forzar a imprimir en consola y limpiar el buffer

        if (semop(id, &v, 1) < 0)
        {
            perror("semop p");
            exit(14);
        }

        sleep(rand() % 2);
    }
}