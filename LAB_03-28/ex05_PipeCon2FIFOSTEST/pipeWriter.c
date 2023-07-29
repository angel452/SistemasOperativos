#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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

	// ########  FOR BUFFER ################

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

	sprintf(bufferVector, "%d", resultado); // Metemos el resultado en el buffer
	int fd;
	char * myfifo;
	// ################# Verificacion par o impar ###########################
	if(resultado % 2 == 0){ // PAR
        
		myfifo = "/tmp/myfifoPAR";
		printf("ES PAR. Ejecutar el ./pipeReaderPAR \n");
		printf("Se enviara el numero: %d\n", resultado);
	}
    	else{ //IMPAR

		//int fdIMPAR;
		myfifo = "/tmp/myfifoIMPAR";
		printf("ES IMPAR. Ejecutar el ./pipeReaderIMPAR\n");
		printf("Se enviara el numero: %d\n", resultado);
    	}	 

	mkfifo(myfifo,0666);
	fd = open(myfifo, O_WRONLY);

	// ### Enviamos informacion  ###
	write(fd, bufferVector, strlen(bufferVector)); // Escribimos en el fifo
	printf("%d %c %d = %d\n", num1, operador, num2, resultado);

	close(fd);
	unlink(myfifo);

}
