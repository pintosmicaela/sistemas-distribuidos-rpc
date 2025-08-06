#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int N, aux1, aux2, fibonacci;
    int sumatoria=0;
    FILE *archivo;

    // Verificar cantidad de argumentos
    if (argc < 3) {
        printf("Uso: %s <numero> <archivo_salida>\n", argv[0]);
        exit(1);
    }
    
    // Convertir el argumento a número entero
    N = atoi(argv[1]);

    // Abrir archivo en modo escritura
    archivo = fopen(argv[2], "w");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    // Manejo de casos base
    if (N == 0) {
        fprintf(archivo, "El valor 0 de Fibonacci es: 0\n");
        fclose(archivo);
        return 0;
    }
    if (N == 1) {
        fprintf(archivo, "El valor 1 de Fibonacci es: 1\n");
        fclose(archivo);
        return 0;
    }

    // Inicialización
    aux1 = 0;
    aux2 = 1;

    // Cálculo
    for (int i = 2; i <= N; i++) {
        fibonacci = aux1 + aux2;
        aux1 = aux2;
        aux2 = fibonacci;
	sumatoria = sumatoria + N;
    }
    // Escribir en archivo
    fprintf(archivo, "El %d-esimo valor de Fibonacci es: %d\n", N, fibonacci);
    fprintf(archivo, "El valor de la sumatoria de los primeros %i numeros es igual a: %i\n", N, sumatoria);
    // Cerrar archivo
    fclose(archivo);
    return 0;
}

