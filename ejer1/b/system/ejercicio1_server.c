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
    char comando[100];

    if (!f) {
        result = 0;
        return &result;
    }
    gettimeofday(&start, NULL);
	snprintf(comando, sizeof(comando), "./tarea %i %s", nombretarea->numero, nombretarea->archivo);
	int estado = system(comando);
	gettimeofday(&end, NULL);
    // Calcular tiempo en microsegundos
    long elapsed_us = (end.tv_sec - start.tv_sec) * 1000000L +
                         (end.tv_usec - start.tv_usec);

    fprintf(f, "Tiempo de ejecución de './tarea': %ld microsegundos ejecutado con system.\n", elapsed_us);
    fclose(f);
    printf("Mensaje impreso en %s\n", nombretarea->archivo);
    result = 1;
    return &result;
}
