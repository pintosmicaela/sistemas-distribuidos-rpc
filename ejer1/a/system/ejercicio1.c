#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	FILE *f;
	struct timeval start, end;
	pid_t pid;
	char comando[100];

	if (argc != 3){
		fprintf(stderr, "Uso: %s <N> <archivo>\n", argv[0]);
		exit(1);
	}
	f = fopen(argv[2], "a");
	if (!f) {
		printf("Error al abrir el archivo\n");
		exit(EXIT_FAILURE);
	}
	gettimeofday(&start, NULL);
	snprintf(comando, sizeof(comando), "./tarea %s %s", argv[1], argv[2]);
	int estado = system(comando);
	if(estado == -1){
		perror("Error al ejecutar ./tarea\n");
		exit(EXIT_FAILURE);
	}
	
	gettimeofday(&end, NULL);
	long elapsed_us = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);
	fprintf(f, "El tiempo de ejecucion de ./tarea: %ld microsegundos ejecutado con execl de forma local\n", elapsed_us);
	fclose(f);
	return 0;
}
