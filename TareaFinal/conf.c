// ################# LIBRARIES #############################
// Libraries for LKM
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

// Libraries for Semaphores
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

// Libraries for Shared Memory
#include <sys/shm.h>

// ################# GLOBAL VARIABLES #############################
// --> For LKM
#define BUFFER_LENGTH 256 ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH]; ///< The receive buffer from the LKM

// --> For Semaphores
#define KEY    0x2222

// --> For Shared Memory
#define SHMSZ   27

// ################# STRUCTURES #############################
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
    int shareMemory_Id;
    key_t key_shm = 1234;
    char *s, *shm;

    int shareMemory_Id2;
    key_t key_shm2 = 9876;
    char *s2, *shm2;

    // --> Crear share memory
    if( (shareMemory_Id = shmget(key_shm, SHMSZ, IPC_CREAT | 0666)) < 0){
        perror("shmget");
        exit(1);
    }

    if ((shareMemory_Id2 = shmget(key_shm2, SHMSZ, IPC_CREAT | 0666)) < 0){
        perror("shmget");
        exit(1);
    }

    // --> Attach
    if ((shm = shmat(shareMemory_Id, NULL, 0)) == (char *) -1){
        perror("shmat");
        exit(1);
    }

    if((shm2 = shmat(shareMemory_Id2, NULL, 0)) == (char *) -1){
        perror("shmat");
        exit(1);
    }

    // ########### SEMAFORAS #####################
    int id = semget(KEY, 1, 0666 | IPC_CREAT);

    // --> Verificar error
    if (id < 0){
        perror("semget");
        exit(11);
    }

    // ########## LKM ############################
    // --> Variables for LKM
    int ret, fd;
    char stringToSend[BUFFER_LENGTH];

    // ######### LOGICA ############
    // --> Inicializamos cantidad de recursos
    union semun recursos;
    recursos.val = 1; // Solo damos 1 recurso, pues solo uno podra escribir a la vez

    // --> Inicializar semafora
    if (semctl(id, 0, SETVAL, recursos) < 0){
        perror("semctl");
        exit(12);
    }

    // --> Asignar valor a la share memory
    s = shm;
    *s = '1';       // " 1 " -> Significa que esta escribiendo el proceso 1
                    // " 2 " -> Significa que esta escribiendo el proceso 2
                    // " 3 " -> Significa que ests escribiendo el proceso 3

    s2 = shm2;
    *s2 = '-';      // "*" -> Significa que alguien esta escribiendo
                    // "-" -> Significa que nadie esta escribiendo

    // --> Abrir LKM
    printf("Creando grupo de WhatsApp...\n");
    fd = open("/dev/UCSP", O_RDWR); // Open the device with read/write access
    if (fd < 0){
        perror("Failed to open the device...");
        return errno;
    }

    // --> Enviar mensaje
    printf("Escriba el mensaje a enviar a todos los miembros: ");
    scanf("%[^\n]%*c", stringToSend);
    ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM

    if(ret < 0){
        perror("Failed to write the message to the device.");
        return errno;
    }

}