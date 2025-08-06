#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	struct timeval start, end;
	pid_t pid;
	FILE *f;
	if (argc <3){
		fprintf(stderr, "Uso: %s <N> <archivo>\n", argv[0]);
		exit(1);
	}
	if (!f) {
		printf("Error al abrir el archivo\n");
		exit(EXIT_FAILURE);
	}
	gettimeofday(&start, NULL);
	pid = fork();
	if (pid < 0){
		perror ("Error al crear el proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) {
		//Proceso hijo
		execl("./tarea", "./tarea", argv[1], argv[2], NULL);
		//Si falla execl
		perror("Error al ejecutar ./tarea\n");
		exit(EXIT_FAILURE);
	}
	else {
		//Proceso padre
		int status;
		waitpid(pid, &status, 0);
		gettimeofday(&end, NULL);
		long elapsed_us = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);
		f = fopen(argv[2], "a");
		if (!f){
			perror("Error al abrir el archivo\n");
			exit(EXIT_FAILURE);
		}
		fprintf(f, "El tiempo de ejecucion de ./tarea: %ld microsegundos ejecutado con execl de forma local\n", elapsed_us);
		fclose(f);
	}
	return 0;
}
