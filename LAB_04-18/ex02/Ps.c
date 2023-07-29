#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define MAXSIZE     128

int flag = 0;

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
  flag = 1;
  printf("Caught signal %d, coming out...\n", signum);
}

void main()
{
  signal(SIGINT, recibirSignal);

  while (1)
  {
    if(flag == 1){

      flag = 0;
      // ######### ID ##############
      int msqid;

      key_t keySendPs;
      keySendPs = 1234;

      // ############ VARIABLES ##############
      struct msgbuf rcvbuffer;

      printf("------------------------------------------------\n");
      // ############ VERIFICADOR DE ERROR ################
      if ((msqid = msgget(keySendPs, 0666)) > 0){
        printf("Recivido\n");
        //die("msgget()");
      }

      if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0) {// FALLO AL RECIBIR 
        die("msgrcv");
      }

      printf("RECIVIDO: %s\n", rcvbuffer.mtext); // SI SE RECIVIO EL MENSAJE
    }
    else{
      printf("Esperando a que se ejecute el programa << P1.exe PID(de Ps.exe) 2 >>\n");
      sleep(3); 
    }
  }
}