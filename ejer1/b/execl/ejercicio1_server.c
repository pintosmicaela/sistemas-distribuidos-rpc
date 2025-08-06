#include <stdio.h>
#include <stdlib.h>
#include "ejercicio1.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

int * ejecutartarea_1_svc(nombre_tarea * nombretarea, struct svc_req * req) {
	static int result;
	FILE *f = fopen(nombretarea->archivo, "a");
	struct timeval start, end;
	pid_t pid;
	char numero_str[10];

	if (!f) {
		result = 0;
		return &result;
	}
	gettimeofday(&start, NULL);
	pid = fork();
	sprintf(numero_str, "%d", nombretarea->numero);

	if (pid < 0){
		perror("Error al crear el proceso hijo");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
                // Proceso hijo
		execl("./tarea", "./tarea", numero_str, nombretarea->archivo, NULL);
		//Si falla excel
		perror("Error al ejecutar ./tarea");
		exit(EXIT_FAILURE);
	} else{
        	// Proceso padre: espera a que termine el hijo
        	int status;
        	waitpid(pid, &status, 0);

        	// Obtener tiempo de finalización
        	gettimeofday(&end, NULL);

        	// Calcular tiempo en microsegundos
        	long elapsed_us = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);
		fprintf(f, "Tiempo de ejecución de './tarea': %ld microsegundos ejecutado con execl\n", elapsed_us);
	}
	fclose(f);
	printf("Mensaje impreso en %s\n", nombretarea->archivo);
	result = 1;
	return &result;
}
