#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#define MAXSIZE     128

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


void main(int argc, char **argv)
{
    int aux1, aux2;
    aux1 = atoi(argv[1]);
    aux2 = atoi(argv[2]);

    // ######### ID ##############
    int msqidS, msqidR, msqidM, msqidD; // id del msq

    // ######### CANTIDAD DE MENSAJES ###########3
    struct msqid_ds bufS;
    struct msqid_ds bufR;
    struct msqid_ds bufM;
    struct msqid_ds bufD;

    key_t keySuma, keyResta, keyMultiplicacion, keyDivicion; // key
    keySuma = 101;
    keyResta = 102;
    keyMultiplicacion = 103;
    keyDivicion = 104;

    // ############ VARIABLES ##############
    struct msgbuf rcvbufferS;
    struct msgbuf rcvbufferR;
    struct msgbuf rcvbufferM;
    struct msgbuf rcvbufferD;

    // ############ VERIFICADOR DE ERROR ################
    if ((msqidS = msgget(keySuma, 0666)) > 0){   //Get the message queue ID for the given key
        printf("Se recivio una Suma\n");
        //die("msgget()");
    }
    printf("msgget Suma\n");
    if ((msqidR = msgget(keyResta, 0666)) > 0){   //Get the message queue ID for the given key
        printf("Se recivio una Resta\n");
        //die("msgget()");
    }
    printf("msgget Resta\n");
    if ((msqidM = msgget(keyMultiplicacion, 0666)) > 0){   //Get the message queue ID for the given key
        printf("Se recivio una Multiplicacion\n");
        //die("msgget()");
    }
    printf("msgget Multiplicacion\n");
    if ((msqidD = msgget(keyDivicion, 0666)) > 0){   //Get the message queue ID for the given key
        printf("Se recivio una Divicion\n");
        //die("msgget()");
    }
    printf("msgget Divicion\n");

    msgctl(msqidS, IPC_STAT, &bufS );
    printf("La cola de mensajes SUMA tiene %ld mensajes.\n", bufS.msg_qnum);

    msgctl(msqidR, IPC_STAT, &bufR );
    printf("La cola de mensajes RESTA tiene %ld mensajes.\n", bufR.msg_qnum);
    
    msgctl(msqidM, IPC_STAT, &bufM );
    printf("La cola de mensajes MULTIPLICACION tiene %ld mensajes.\n", bufM.msg_qnum);

    msgctl(msqidD, IPC_STAT, &bufD );
    printf("La cola de mensajes DIVICION tiene %ld mensajes.\n", bufD.msg_qnum);


    // ############ VERIFICADOR DE ERROR ################
    /*
    OJO: el 1 es el mtype que estamos enviando en el send(), tenemos que cambiarlo para lo que nosotros queramos
    Para convertir de num a texto -> sprintf
    */
    if(bufS.msg_qnum != 0){
        if (msgrcv(msqidS, &rcvbufferS, MAXSIZE, 1, 0) > 0){ // FALLO AL RECIBIR 
            printf("Mensaje recivido para Sumar\n");
            //die("msgrcv");
        }
    }
    else if(bufR.msg_qnum != 0){
        if (msgrcv(msqidR, &rcvbufferR, MAXSIZE, 2, 0) > 0){ // FALLO AL RECIBIR 
            printf("Mensaje recivido para Restar\n");
            //die("msgrcv");
        }
    }
    else if(bufM.msg_qnum != 0){
        if (msgrcv(msqidM, &rcvbufferM, MAXSIZE, 3, 0) > 0){ // FALLO AL RECIBIR 
            printf("Mensaje recivido para Multiplicar\n");
            //die("msgrcv");
        }
    }
    else if(bufD.msg_qnum != 0){
        if (msgrcv(msqidD, &rcvbufferD, MAXSIZE, 4, 0) > 0){ // FALLO AL RECIBIR 
            printf("Mensaje recivido para Dividir\n");
            //die("msgrcv");
        }
    }
    // ################# OPERACIONES ##########################

    int auxOperacion;

    printf("Se recivio %s. Sumando...\n", rcvbufferS.mtext); // SI SE RECIVIO EL MENSAJE
    auxOperacion = atoi(rcvbufferS.mtext);
    auxOperacion = auxOperacion + 3;
    sprintf(rcvbufferS.mtext, "%d", auxOperacion);

    printf("Se recivio %s. Restando...\n", rcvbufferR.mtext); // SI SE RECIVIO EL MENSAJE
    auxOperacion = atoi(rcvbufferR.mtext);
    auxOperacion = auxOperacion - 7;
    sprintf(rcvbufferR.mtext, "%d", auxOperacion);
    
    printf("Se recivio %s. Multiplicando...\n", rcvbufferM.mtext); // SI SE RECIVIO EL MENSAJE
    auxOperacion = atoi(rcvbufferM.mtext);
    auxOperacion = auxOperacion * 6;
    sprintf(rcvbufferM.mtext, "%d", auxOperacion);

    printf("Se recivio %s. Dividiendo...\n", rcvbufferD.mtext); // SI SE RECIVIO EL MENSAJE
    auxOperacion = atoi(rcvbufferD.mtext);
    auxOperacion = auxOperacion / 3;
    sprintf(rcvbufferD.mtext, "%d", auxOperacion);

    //#################################################################################

    printf("RESULTADO... S: %s, R: %s, M: %s, D %s\n", rcvbufferS.mtext, rcvbufferR.mtext, rcvbufferM.mtext, rcvbufferD.mtext);

    int msquidSendPs;
    key_t keySendPs;
    keySendPs = 1234;

    // ########## PERMISOS ################
    int msgflg = IPC_CREAT | 0666; // con permiso 0666

    // ############ VARIABLES ##############
    struct msgbuf sbufSendPs;
    size_t buflenSendPs;

    // ############ VERIFICADOR DE ERROR ################
    // Llamamos a la funcion msget. Si el resultado es -1, mandamos error. Pero si no, almacenamos en msqid el valor que retorna msgget() y eso lo utilizamos en el msgsend()
    if ((msquidSendPs = msgget(keySendPs, msgflg )) < 0)   //Get the message queue ID for the given key
        die("msgget");
    
    sbufSendPs.mtype = 1;

    if(msqidS > 0){
        int aux = atoi(rcvbufferS.mtext);
        sprintf(sbufSendPs.mtext, "%d", aux); // Metemos el resultado en el buffer
    }
    if(msqidR > 0){
        int aux = atoi(rcvbufferR.mtext);
        sprintf(sbufSendPs.mtext, "%d", aux); // Metemos el resultado en el buffer
    }
    if(msqidM > 0){
        int aux = atoi(rcvbufferM.mtext);
        sprintf(sbufSendPs.mtext, "%d", aux); // Metemos el resultado en el buffer
    }
    if(msqidD > 0){
        int aux = atoi(rcvbufferD.mtext);
        sprintf(sbufSendPs.mtext, "%d", aux); // Metemos el resultado en el buffer
    }

    buflenSendPs = strlen(sbufSendPs.mtext) + 1 ; // es el tamaño del buff

    // ############ VERIFICADOR DE ERROR ################
    if (msgsnd(msquidSendPs, &sbufSendPs, buflenSendPs, IPC_NOWAIT) < 0) // NO PASO
    {
        printf ("%d, %ld, %s, %d \n", msquidSendPs, sbufSendPs.mtype, sbufSendPs.mtext, (int)buflenSendPs);
        die("msgsnd");
    }
    else
        printf("Message Sent\n"); // SE ENVIO EL MENSAJE

    kill(aux1, aux2);

    exit(0);
}

