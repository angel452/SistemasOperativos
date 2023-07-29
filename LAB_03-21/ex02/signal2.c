#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void enviarSignal(int signum) {
     printf("Caught signal %d, coming out...\n", signum);
  }

void enviarSignal2(int signum){
	printf("Caugth signal %d, coming out...\n", signum);
}

int main () {

   signal(SIGINT, enviarSignal); //SIGINT es para RECIVIR una signalizacion
   			         //enviarSignal es la funcion que usaremos y creamos

   while(1) {
      printf("Going to sleep for a second...\n");
      sleep(1); 
   }
   return(0);
}

