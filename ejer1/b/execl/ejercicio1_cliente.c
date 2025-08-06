#include <stdio.h>
#include <stdlib.h>
#include "ejercicio1.h"

int main(int argc, char *argv[]) {
    CLIENT * clnt;
    nombre_tarea nombretarea;
    int * result;

    if (argc != 4) {
        fprintf(stderr, "Uso: %s <host> <archivo> <N>\n", argv[0]);
        exit(1);
    }

    clnt = clnt_create(argv[1], EJECUTARTAREAPROG, EJECUTARTAREAVERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(argv[1]);
        exit(1);
    }

    nombretarea.archivo = argv[2];  // asignamos el mensaje al campo texto del struct
    nombretarea.numero = atoi(argv[3]);
    result =  ejecutartarea_1(&nombretarea, clnt);

    if (result == NULL) {
        clnt_perror(clnt, argv[1]);
        exit(1);
    }

    if (* result == 1)
        printf("Mensaje entregado con éxito.\n");
    else
        printf("El servidor no pudo imprimir el mensaje.\n");

    clnt_destroy(clnt);
    return 0;
}
