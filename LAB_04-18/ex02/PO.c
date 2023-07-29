#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#define MAXSIZE     128

// ############# Creamos variables para la signalizacion ##########
int flag = 0;

// ################## FUNCIONES PARA EL IPCS ##############
void die(char *s)
{
    perror(s);
    exit(1);
}

struct msgbuf
{
    long    mtype; // podemos usar este para la suma (1), resta (2), etc.
    char    mtext[MAXSIZE];
};

void enviarSignal(int signum) {
    
    flag = 1;
    printf("-------------------------------------------------------------------- \n");
    printf("Flag = 1 -> Signalizacion numero %d recivida.\n", signum);
}

int main(){

    // ############### FOR SIGNAL #########################
    signal(SIGINT, enviarSignal); //SIGINT es para RECIVIR una signalizacion
   			                      //enviarSignal es la funcion que usaremos y creamos

    // ######### ID ##############
    int msqid; // id del msq
    
    key_t keySuma, keyResta, keyMultiplicacion, keyDivicion; // key
    keySuma = 101;
    keyResta = 102;
    keyMultiplicacion = 103;
    keyDivicion = 104;

    // ########## PERMISOS ################
    int msgflg = IPC_CREAT | 0666; // con permiso 0666

    // ############ VARIABLES ##############
    struct msgbuf sbuf;
    size_t buflen;
    int randomOperation, randomNumber;
    //char bufferVector[100];

    while (1){
        if(flag == 1){

            flag = 0;

            //############# LOGICA DE NUMEROS ALEATORIOS ##############
            srand(time(NULL));
            randomOperation = (rand()%4)+1;
            randomNumber = rand()%1001;
            
            printf("Numero aleatorio: %d\n", randomNumber); 
            printf("Operacion: %d", randomOperation); 

            // ############ MSGGET and mtype ##########################
            if(randomOperation == 1){

                if ((msqid = msgget(keySuma, msgflg)) < 0){   //Get the message queue ID for the given key
                    die("msgget");
                }

                printf(" -> Suma\n");
                sbuf.mtype = 1;
            }
            else if(randomOperation == 2){

                if ((msqid = msgget(keyResta, msgflg)) < 0){   //Get the message queue ID for the given key
                    die("msgget");
                }

                printf(" -> Resta\n");
                sbuf.mtype = 2;
            }
            else if(randomOperation == 3){

                if ((msqid = msgget(keyMultiplicacion, msgflg)) < 0){   //Get the message queue ID for the given key
                    die("msgget");
                }

                printf(" -> Multiplicacion\n");
                sbuf.mtype = 3;
            }
            else if(randomOperation == 4){

                if ((msqid = msgget(keyDivicion, msgflg)) < 0){   //Get the message queue ID for the given key
                    die("msgget");
                }
            
                printf(" -> Divicion\n");
                sbuf.mtype = 4;
            }

            //############# CONFIGURACIONES ###################
            sprintf(sbuf.mtext, "%d", randomNumber); // Metemos el resultado en el buffer
            buflen = strlen(sbuf.mtext)+1; 

            // ############ VERIFICADOR DE ERROR (send) ################
            if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0) // NO PASO
            {
                printf ("%d, %ld, %s, %d \n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
                die("msgsnd");
            }
            else{
                printf("Message Sent\n"); // SE ENVIO EL MENSAJE
            }
            
            //exit(0);
        }
        else{
            printf("Esperando signalizacion. Flag = %d\n", flag);
            sleep(3); 
        }
    }
    
    return 0;

    
}