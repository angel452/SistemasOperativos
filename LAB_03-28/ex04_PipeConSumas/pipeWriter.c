#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Objetivo: hacer un programa prg.exe que reciva 33, 15 y un operador, 
// estos se suman y deberá enviarlo a otro programa Reader que luego lo sumara +10 y eso imprime

int main(int argc, char *argv[]){
		
	// ################ Recibimos parametros ##################3
	if (argc != 4) {
        printf("Uso: %s <numero> <operador> <numero>\n", argv[0]);
        return 1;
    }

	int num1, num2;

	num1 = atoi(argv[1]);
	num2 = atoi(argv[3]);

	char operador;
	operador = argv[2][0];

	// ########  FOR FIFO ################
	int fd;
	char * myfifo = "/tmp/myfifo";
	
	mkfifo(myfifo,0666);
	fd = open(myfifo, O_WRONLY);

	char bufferVector[100]; // En el bufferVector guardaremos la respuesta
						    // Por algun motivo no se puede enviar un int
	for(int i = 0; i < 100; i++){
		bufferVector[i] = '\n';
	}
	// ########### Operaciones #################

	int resultado;
	switch (operador) {
	case '+':
		resultado = num1 + num2;
		//write(fd, resultado, sizeof(resultado));
		break;
	case '-':
		resultado = num1 - num2;
		//write(fd, resultado, sizeof(resultado));
		break;
	case '*':
		resultado = num1 * num2;
		//write(fd, resultado, sizeof(resultado));
		break;
	case '/':
		if (num2 == 0) {
			printf("No se puede dividir por cero\n");
			return 1;
		}
		resultado = num1 / num2;
		//write(fd, resultado, sizeof(resultado));
		break;
	default:
		printf("Operador invalido: %c\n", operador);
		return 1;
	}

	// ########### Enviamos informacion  #################
	sprintf(bufferVector, "%d", resultado); // Metemos el resultado en el buffer
	write(fd, bufferVector, strlen(bufferVector)); // Escribimos en el fifo

	printf("%d %c %d = %d\n", num1, operador, num2, resultado);
	printf("Se enviara el numero: %d\n", resultado);
	printf("Al programa pipePlus10.c");

	close(fd);
	unlink(myfifo);
	 
}
