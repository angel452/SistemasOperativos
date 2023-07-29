#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
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


void main()
{
  // ######### ID ##############
  int msqid; // id del msq

  key_t key;  // keY
  key = 1234;

  // ############ VARIABLES ##############
  struct msgbuf rcvbuffer;

  // ############ VERIFICADOR DE ERROR ################
  if ((msqid = msgget(key, 0666)) < 0)
    die("msgget()");


  // ############ VERIFICADOR DE ERROR ################
  /*
  OJO: el 1 es el mtype que estamos enviando en el send(), tenemos que cambiarlo para lo que nosotros queramos
  Para convertir de num a texto -> sprintf
  */
  if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0) // FALLO AL RECIBIR 
    die("msgrcv");

  printf("%s\n", rcvbuffer.mtext); // SI SE RECIVIO EL MENSAJE
  exit(0);
}

