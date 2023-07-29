#include <stdio.h>
#include <time.h>
#include <conio.h>

int main() {
    printf("Ingresa algo en los próximos 5 segundos:\n");
    
    clock_t start_time = clock();
    clock_t current_time;
    int elapsed_seconds;
    
    while (1) {
        if (kbhit()) {
            getchar(); // Leer y descartar el carácter ingresado
            printf("Ingresaste algo en menos de 5 segundos.\n");
            break;
        }
        
        current_time = clock();
        elapsed_seconds = (current_time - start_time) / CLOCKS_PER_SEC;
        
        if (elapsed_seconds >= 5) {
            printf("No ingresaste nada en 5 segundos.\n");
            break;
        }
    }
    
    return 0;
}