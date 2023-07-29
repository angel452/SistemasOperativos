#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

main()
{
  // ######### ID ##############
  int msqid; // id del msq

  key_t key; // key
  key = 1234;

  // ########## PERMISOS ################
  int msgflg = IPC_CREAT | 0666; // con permiso 0666

  // ############ VARIABLES ##############
  struct msgbuf sbuf;
  size_t buflen;

  // ############ VERIFICADOR DE ERROR ################
  // Llamamos a la funcion msget. Si el resultado es -1, mandamos error. Pero si no, almacenamos en msqid el valor que retorna msgget() y eso lo utilizamos en el msgsend()
  if ((msqid = msgget(key, msgflg )) < 0)   //Get the message queue ID for the given key
    die("msgget");

  //############# CONFIGURACIONES ###################
  sbuf.mtype = 1;

  // ############# MENSAJE ##################
  printf("Enter a message to add to message queue : ");
  scanf("%[^\n]",sbuf.mtext);
  getchar();

  buflen = strlen(sbuf.mtext) + 1 ; // es el tamaño del buff

  // ############ VERIFICADOR DE ERROR ################
  if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0) // NO PASO
  {
      printf ("%d, %ld, %s, %d \n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
      die("msgsnd");
  }
  else
      printf("Message Sent\n"); // SE ENVIO EL MENSAJE

  exit(0);
}

