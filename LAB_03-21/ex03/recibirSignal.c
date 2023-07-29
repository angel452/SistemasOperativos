#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void recibirSignal(int signum) {
     printf("Caught signal %d, coming out...\n", signum);
  }

int main () {

   signal(SIGINT, recibirSignal);

   while(1) {
      printf("Going to sleep for a second...\n");
      sleep(1); 
   }
   return(0);
}
