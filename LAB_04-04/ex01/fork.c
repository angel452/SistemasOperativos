#include<stdio.h>

int main() {

   int pid; //process ID
   pid = fork(); // Aqui se genera el hijo y el padre

   // Preguntamos si es padre o es hijo
   if (pid == 0) { // PARA HIJO
      system("echo 'hijo'");
      system("echo $$"); // $$ es para sacar el ID
      sleep(100);
   } 
   else { // PARA PADRE
      system("echo 'padre'");
      system("echo $$"); // $$ es para sacar el ID
      sleep(100);
   }
   return 0;
}

