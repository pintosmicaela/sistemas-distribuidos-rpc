#include "ejercicio2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

double tiempo_en_sg(){
	struct timeval tiempo;
	gettimeofday(&tiempo, NULL);
	return tiempo.tv_sec + tiempo.tv_usec / 1000000.0;
}

int main(int argc, char *argv[]){
	CLIENT *clnt;
	arg_input arginput;
	arg_result *argresult;
	double total = 0.0;

	if(argc != 2){
		printf("Uso: %s <hostname>\n", argv[0]);
		exit(1);
	}

	clnt = clnt_create(argv[1], EJERCICIO2PROG, EJERCICIO2VERS, "tcp");
	if(clnt == NULL){
		clnt_pcreateerror(argv[1]);
		exit(1);
	}

	double comienzo = tiempo_en_sg();
	argresult = remote_call_1(&arginput, clnt);
	double fin = tiempo_en_sg();

	if(argresult == NULL){
		clnt_perror(clnt, "error en la llamada\n");
		exit(1);
	}

	total += (fin - comienzo);
	printf("Tiempo del procedimiento de forma remota:  %f ms\n", total);
	clnt_destroy(clnt);
	return 0;
}
