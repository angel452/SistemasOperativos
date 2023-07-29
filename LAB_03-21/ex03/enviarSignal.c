#include <stdio.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int aux1, aux2;
	
	aux1 = atoi(argv[1]);
	aux2 = atoi(argv[2]);

	kill(aux1, aux2);
}


