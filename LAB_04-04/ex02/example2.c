#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

// Objetivo: Enviar un mensaje desde el hijo, y que lo reciva el padre, aumentandole algo mas 

int main(){

    int fd[2];
    // ########################################################
    //fdp[0] = read / input / recivir
    //        Cerramos lectura pues vamos a escribir
    //        close(fdp[0]);
    //        write(fdp[1], string, (strlen(string)+1));
    //        close(fsp[1])

    //fdp[1] = write / output / enviar
    //       Cerramos escritura pues vamos a leer
    //       close(fdp[1]);
    //       read(fdp[0], readbuffer, sizeof(readbuffer));
    //       close(fd[0]);
    // ########################################################
    pipe(fd);

    //una vez creado el pipe, podemos hacer fork para que ambos procesos tambien tengan el pipe
    
    int id = fork();
    if( id == 0 ){ // Hijo
        close(fd[0]);
        int num;
        printf("Ingrese un numero: \n");
        scanf("%d", &num);

        // como enviamos este numero al padre?
        write(fd[1], &num, sizeof(int));
        close(fd[1]);
    }
    else{
        close(fd[1]);

        // Ya que se envio el mensaje del hijo, como padre tenemos que leerlo
        int numRecv;
        read (fd[0], &numRecv, sizeof(int));
        numRecv = numRecv + 10;
        close(fd[0]);
        printf("Se obtuvo: %d, del hijo \n", numRecv);
    }
}