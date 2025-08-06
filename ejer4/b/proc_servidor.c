#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // gethostname
#include <time.h>
#include "mini_servicios.h"

char **obtener_nombre_maquina_1_svc(void *argp, struct svc_req *rqstp) {
    	static char *resultado;
    	static char buffer[256];  // Espacio para el hostname
    	if (gethostname(buffer, sizeof(buffer)) != 0) {
        	printf("Error al obtener el nombre de la máquina");
        	resultado = "";
    	} else {
        	resultado = buffer;
   	 }

	 return &resultado;
}
char ** obtener_hora_sistema_1_svc( void *argp, struct svc_req * rqstp){
	static char * resultado;
	static char buffer[256];
	time_t tiempo_actual = time(NULL);
   	struct tm *info_tiempo = localtime(&tiempo_actual);
   	strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", info_tiempo);
	resultado=buffer;
	return &resultado;
}
char **convertir_decimal_a_binario_1_svc(entero_param *argp, struct svc_req *rqstp) {
	 static char *resultado;
    static char buffer[33];
    int valor = argp->valor;

    memset(buffer, 0, sizeof(buffer));

    if (valor == 0) {
        strcpy(buffer, "0");
        resultado = buffer;
    } else {
        int index = 32;
        buffer[index] = '\0';
        while (valor > 0 && index > 0) {
            buffer[--index] = (valor % 2) ? '1' : '0';
            valor /= 2;
        }
        resultado = &buffer[index];
    }

    return &resultado;
}

int * multiplicar_1_svc(multiplicacion_param * argp, struct svc_req * rqstp){
	static int resultado;
	int cuenta  = argp->operando1 * argp->operando2 * argp->operando3 * argp->operando4 ;
	resultado = cuenta;
	return &resultado;	
}
int * sumar_1_svc(suma_param * argp, struct svc_req * rqstp){
        static int resultado;
        int cuenta  = argp->operando1 + argp->operando2 + argp->operando3 + argp->operando4 ;
        resultado = cuenta;
        return &resultado;      
}
