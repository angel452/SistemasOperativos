#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#define MAXSIZE     128
#define SHMSZ     27 // Tamaño d e la share memory

// ################## FUNCIONES PARA EL IPCS ##############
void die(char *s)
{
  perror(s);
  exit(1);
}

struct msgbuf
{
  long    mtype;
  char    mtext[MAXSIZE];
};

void recibirSignal(int signum) {

    // ####################### RECIVIR EL NUMERO ####################

    printf("Caught signal %d, coming out...\n", signum);
        int shmid; // id del shm
    key_t key; // llave
    char *shm, *s;

    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 5678;

    /*
     * Locate the segment.
     */
    // a diferencia del otro programa, este ya no llama al IPC_CREAT, pues ya esta creado
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) { 
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    // USamos atach para recuperar 
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * Now read what the server put in the memory.
     */
    for (s = shm; *s != NULL; s++)
        putchar(*s);
    putchar('\n');

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shm = '*'; // ponemos un asterisco en la primera posicion para que pare el otro programa

    exit(0);

}

int main(int argc, char *argv[]){
		
    signal(SIGINT, recibirSignal);
	// ################ Recibimos parametros ##################3
	if (argc != 3) {
        printf("Uso: %s <numero> <numero>\n", argv[0]);
        return 1;
    }

	int num1, num2;

	num1 = atoi(argv[1]);
	num2 = atoi(argv[2]);

    //printf("Numeros recividos: %d  y %d \n", num1, num2);
    printf("Numeros recividos: %d \n", num1);

    // ######### ID ##############
    int msqid1; // id del msq

    key_t key1;
    key1 = 1234;

    // ########## PERMISOS ################
    int msgflg = IPC_CREAT | 0666; // con permiso 0666

    // ############ VARIABLES ##############
    struct msgbuf sbuf1;
    size_t buflen1;


    // ############ VERIFICADOR DE ERROR ################
    // Llamamos a la funcion msget. Si el resultado es -1, mandamos error. Pero si no, almacenamos en msqid el valor que retorna msgget() y eso lo utilizamos en el msgsend()
    if ((msqid1 = msgget(key1, msgflg )) < 0)   //Get the message queue ID for the given key
        die("msgget");

    //############# CONFIGURACIONES ###################
    sbuf1.mtype = 1;

    sprintf(sbuf1.mtext, "%d", num1);
    buflen1 = strlen(sbuf1.mtext)+1;

    // ############ VERIFICADOR DE ERROR (send) ################
    if (msgsnd(msqid1, &sbuf1, buflen1, IPC_NOWAIT) < 0) // NO PASO
    {
        printf ("%d, %ld, %s, %d \n", msqid1, sbuf1.mtype, sbuf1.mtext, (int)buflen1);
        die("msgsnd");
    }
    else
        printf("Message Sent 1\n"); // SE ENVIO EL MENSAJE

    //exit(0);
    
    while(1) {
        printf("Esperando el share memory\n");
        sleep(1); 
    }
    return(0);
    
}
