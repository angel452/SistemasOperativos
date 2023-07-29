#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

// ############# Creamos variables para la signalizacion ##########
int flag = 0;

// ###### Creamos los FILE DESCRIPTOR ###########
int FDP1[2], FDP2[2]; 
pid_t IDhijo1, IDhijo2; //id del hijo/padre

// ###### Creamos el pipe ################
pipe(FDP1);
pipe(FDP2);

// ############# Creamos variables para leer la respuesta en los hijos ##########
int randomNumber;
char bufferVector[100];

void enviarSignal(int signum) {
    
    flag = 1;
    printf("-------------------------------------------------------------------- \n");
    printf("Flag = 1 -> Signalizacion numero %d recivida.\n", signum);
}

int main () {
    // ############### FOR FIFO #########################
    int fd;
    char * myfifo;
    myfifo = "/tmp/myfifo";
    for(int i = 0; i < 100; i++){
		bufferVector[i] = '\n';
	}

    // ############### FOR SIGNAL #########################
    signal(SIGINT, enviarSignal); //SIGINT es para RECIVIR una signalizacion
   			                      //enviarSignal es la funcion que usaremos y creamos

    // ############### FORKS #########################
    printf("CREANDO HIJOS \n");
            
    IDhijo1 = fork();
    if(IDhijo1 == 0){ // Es un hijo
        close(FDP1[1]); // cerramos el escritura pues vamos a leer 

        int numRecv;
        //read(FDP1[0], readbuffer, sizeof(readbuffer));
        read(FDP1[0], &numRecv, sizeof(int));
        close(FDP1[0]);

        printf("Se obtuvo: %d, del hijo \n", numRecv);

        // OPERACION RESPECTIVA
        //numRecv = pow(numRecv, 3);
        numRecv = numRecv * 2;

        //fifo
        mkfifo(myfifo, 0666);
        fd = open(myfifo, O_WRONLY);
        sprintf(bufferVector, "%d", numRecv); // Metemos el resultado en el buffer
        write(fd, bufferVector, strlen(bufferVector));
        close(fd);
        unlink(myfifo);
    }

    if(IDhijo1 != 0){ // Eso quiere decir que estamos en el proceso padre. Y el padre es el que unicamente debe crear un hijo mas. No el anterior hijo
        IDhijo2 = fork();
        if(IDhijo2 == 0){ // Es un hijo
            close(FDP2[1]);

            int numRecv;
            read(FDP2[0], &numRecv, sizeof(int));
            close(FDP1[0]);

            printf("Se obtuvo: %d, del hijo \n", numRecv);

            // OPERACION RESPECTIVA
            numRecv = numRecv / 2;

            //fifo
            mkfifo(myfifo, 0666);
            fd = open(myfifo, O_WRONLY);
            sprintf(bufferVector, "%d", numRecv); // Metemos el resultado en el buffer
            write(fd, bufferVector, strlen(bufferVector));
            close(fd);
	        unlink(myfifo);
        }
    }

    while(1) {
        if(flag == 1){ // Se recive una señal

            flag = 0;
            
            // ########### CREAMOS NUMERO ALEATORIO #############
            srand(time(NULL));
            //int randomNumber = rand()%1001;
            //randomNumber = rand()%1001;
            randomNumber = rand()%11;
            printf("Numero creado: %d\n", randomNumber);
            
            if(randomNumber % 2 == 0){ 
                
                printf("ES PAR\n");
                close(FDP1[0]);
                write(FDP1[1], &randomNumber, sizeof(int));
                close(FDP1[1]);
            }

            else { 

                printf("ES IMPAR \n");
                close(FDP2[0]);
                write(FDP2[1], &randomNumber, sizeof(int));
                close(FDP2[1]);
            }

            printf("-------------------------------------------------------------------- \n");
        }
        else{
            printf("Esperando signalizacion. Flag = %d\n", flag);
            sleep(5); 
        }
    }
    return(0);
}
