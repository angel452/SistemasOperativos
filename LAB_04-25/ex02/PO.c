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

#define SHMSZ     27 // Tamaño de la shm (share memory)

// ############# Creamos variables para la signalizacion ##########
int flag = 0;

void enviarSignal(int signum) {
    
    flag = 1;
    printf("-------------------------------------------------------------------- \n");
    printf("Flag = 1 -> Signalizacion numero %d recivida.\n", signum);
}

int main(){

    // ############ VARIABLES ##############
    int randomOperation, randomNumber;

    char operador, numero; 
    char *shm, *s;

    // ######### ID ##############
    int shmid;
    key_t keySuma, keyResta, keyMultiplicacion, keyDivicion; // key
    keySuma = 101;
    keyResta = 102;
    keyMultiplicacion = 103;
    keyDivicion = 104;

    // ############### FOR SIGNAL #########################
    signal(SIGINT, enviarSignal); //SIGINT es para RECIVIR una signalizacion
   			                      //enviarSignal es la funcion que usaremos y creamos

    while(1){
        if(flag == 1){
            //############# LOGICA DE NUMEROS ALEATORIOS ##############
            srand(time(NULL));
            randomOperation = (rand()%4)+1;
            randomNumber = rand()%1001;

            printf("Numero aleatorio: %d\n", randomNumber); 
            printf("Operacion: %d", randomOperation); 

            // ############ MSGGET and mtype ##########################
            if(randomOperation == 1){
                
                /*
                * Create the segment.
                */
                if((shmid = shmget(keySuma,SHMSZ, IPC_CREAT | 0666)) < 0){
                    perror("shmget");
                    exit(1);
                }
                operador = '1';
                printf(" -> Suma\n");
            }
            else if(randomOperation == 2){

                /*
                * Create the segment.
                */
                if((shmid = shmget(keyResta,SHMSZ, IPC_CREAT | 0666)) < 0){
                    perror("shmget");
                    exit(1);
                }
                operador = '2';
                printf(" -> Resta\n");
            }
            else if(randomOperation == 3){

                /*
                * Create the segment.
                */
                if((shmid = shmget(keyMultiplicacion,SHMSZ, IPC_CREAT | 0666)) < 0){
                    perror("shmget");
                    exit(1);
                }
                operador = '3';
                printf(" -> Multiplicacion\n");
            }
            else if(randomOperation == 4){
                
                /*
                * Create the segment.
                */
                if((shmid = shmget(keyDivicion,SHMSZ, IPC_CREAT | 0666)) < 0){
                    perror("shmget");
                    exit(1);
                }
                operador = '4';
                printf(" -> Divicion\n");
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
            s = shm; // inicializamos s en el comienzo
            *s = operador;
            *s++ = numero;
            *s = NULL;

            /*
            while (*shm != '*')
                leep(1);
            */
        }
        else{
            printf("Esperando signalizacion. Flag = %d\n", flag);
            sleep(3); 
        }
    }
}