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

#include <stdbool.h>

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

// ################# MAIN #############################
int main(){

    // -------------- VARIABLES ------------------
    // --> Variables for LKM
    int ret, fd;
    char stringToSend[BUFFER_LENGTH];

    // --> Variables for Semaphores
    int semafora_id = semget(KEY, 1, 0666 | IPC_CREAT);
    if(semafora_id < 0){
        perror("semget");
        exit(11);
    }

    // --> Variables for Shared Memory
    int shareMemory_id;
    key_t key_shm = 1234;
    char *s, *shm;

    int shareMemory_id2;
    key_t key_shm2 = 9876;
    char *s2, *shm2;

    //Localizamos el share memory
    if ((shareMemory_id = shmget(key_shm, SHMSZ, 0666)) < 0){
        perror("shmget");
        exit(1);
    }

    if ((shareMemory_id2 = shmget(key_shm2, SHMSZ, 0666)) < 0){
        perror("shmget");
        exit(1);
    }

    // Attach
    if ((shm = shmat(shareMemory_id, NULL, 0)) == (char *) -1){
        perror("shmat");
        exit(1);
    }

    if ((shm2 = shmat(shareMemory_id2, NULL, 0)) == (char *) -1){
        perror("shmat");
        exit(1);
    }
    
    // ----------------- LOGIC --------------------
    printf("\n");
    printf( "\\ \\        / / |         | |         /\\                \n");
    printf( " \\ \\  /\\  / /| |__   __ _| |_ ___   /  \\   _ __  _ __  \n");
    printf( "  \\ \\/  \\/ / | '_ \\ / _` | __/ __| / /\\ \\ | '_ \\| '_ \\ \n");
    printf( "   \\  /\\  /  | | | | (_| | |_\\__ \\/ ____ \\| |_) | |_) |\n");
    printf( "    \\/  \\/   |_| |_|\\__,_|\\__|___/_/    \\_\\ .__/| .__/ \n");
    printf( "                                          | |   | |    \n");
    printf( "                                          |_|   |_|    \n");    
    printf("\n");

    fd = open("/dev/UCSP", O_RDWR); // Open the device with read/write access
    if (fd < 0){
        perror("Failed to open the device...");
        return errno;
    }
    
    // --> Leemos lo que tiene el kernel
    printf("BTW... Someone send a message ... \n");
    ret = read(fd, receive, BUFFER_LENGTH); // Read the response from the LKM
    if (ret < 0){
        perror("Failed to read the message from the device.");
        return errno;
    }
    printf("The received message is: [%s]\n", receive);
    printf("\n");

    while (1)
    {
        // --> Lock
        if(semop(semafora_id, &p, 1) < 0){
            perror("semop");
            exit(12);
        }

        // --> Leemos Shared Memory
        s = shm;
        s2 = shm2;

        if ( *s == '1'){

            if(*s2 == '*'){
                // --> Leemos lo que tiene el kernel
                printf("You have new messages ... \n");
                ret = read(fd, receive, BUFFER_LENGTH); // Read the response from the LKM
                if (ret < 0){
                    perror("Failed to read the message from the device.");
                    return errno;
                }
                printf("The received message is: [%s]\n", receive);
                printf("\n");
                sleep(1);
            }

            printf("|-------------------------------------|\n");
            printf("| Send Message ............ ENTER     |\n");
            printf("| Exit .................... Ctrl + C  |\n");
            printf("|-------------------------------------|\n");
            printf("\n");
            printf("Waiting ... \n");
            getchar();

            *s2 = '*';
            *s = '2'; 

            // --> Brutal Force to clean the buffer
            for(int i = 0; i < BUFFER_LENGTH; i++){
                stringToSend[i] = '_';
            }

            printf("Type something to your group: \n");
            scanf("%[^\n]%*c", stringToSend);
            ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
            if (ret < 0){
                perror("Failed to write the message to the device.");
                return errno;
            }
            printf("Message is sent: [%s]\n", stringToSend);

            // --> Unlock
            if(semop(semafora_id, &v, 1) < 0){
                perror("semop");
                exit(12);
            }
        }
        else{
            if(*s2 == '*'){
                // --> Leemos lo que tiene el kernel
                printf("You have new messages ... \n");
                ret = read(fd, receive, BUFFER_LENGTH); // Read the response from the LKM
                if (ret < 0){
                    perror("Failed to read the message from the device.");
                    return errno;
                }
                printf("The received message is: [%s]\n", receive);
                printf("\n");
                sleep(1);
            }

            // --> Unlock
            if(semop(semafora_id, &v, 1) < 0){
                perror("semop");
                exit(12);
            }
        }
    }
    return 0;
}