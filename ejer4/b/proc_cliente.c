#include <stdio.h>
#include <stdlib.h>
#include "mini_servicios.h"

int main(int argc, char *argv[]) {
    	CLIENT * clnt;
    	char * * string_rta;
    	int numero_servicio;
	struct timeval inicio, fin;		



	if (argc != 2) {
        	fprintf(stderr, "Uso: %s <host>\n", argv[0]);
        	exit(1);
    	}	
	
	printf("Ingrese el número de servicio.\n");
    	printf("1. obtener nombre de la maquina.\n");	
	printf("2. obtener fecha y hora del sistema.\n");
	printf("3. convertior un entero decimal a binario\n");
	printf("4. si desea realizar una multiplicacion\n");
	printf("5. si desea realizar una suma\n");
	scanf("%i", &numero_servicio);
	
	clnt = clnt_create(argv[1], MINISERVICIOS, SERVICIOSVERS, "udp");
        if (clnt == NULL) {
        	clnt_pcreateerror(argv[1]);
                exit(1);
        }

	if(numero_servicio == 1){
		gettimeofday(&inicio, NULL);
		string_rta = obtener_nombre_maquina_1(NULL, clnt);
		gettimeofday(&fin, NULL);
		if(string_rta == NULL){
			printf("Error al llamar al servicio obtener_nombre_maquina_1");
			exit(-1);
		}
		printf("El nombre del host es: %s\n", *string_rta);		
	}
	if(numero_servicio == 2){
                gettimeofday(&inicio, NULL);
		string_rta = obtener_hora_sistema_1(NULL, clnt);
		gettimeofday(&fin, NULL);
                if(string_rta == NULL){
                        printf("Error al llamar al servicio obtener_hora_sistema_1");
                        exit(-1);
                }
                printf("La hora del sistema es: %s\n", *string_rta);            
        }
	
	if(numero_servicio == 3){
		entero_param entero_decimal;	
		
    		printf("Ingrese el entero decimal: ");
    		scanf("%d", &entero_decimal.valor);		

		gettimeofday(&inicio, NULL);
                string_rta = convertir_decimal_a_binario_1(&entero_decimal, clnt);
		gettimeofday(&fin, NULL);

                if(string_rta == NULL){
                        printf("Error al llamar al servicio convertir_decimal_a_binario_1");
                        exit(-1);
                }
                printf("El valor ingresado expresado en binario es igual a: %s\n", *string_rta);	
	}
	if(numero_servicio ==  4){
		multiplicacion_param parametros = {1,1,1,1};  // Inicializa todo a 0
    		int cantidad, valor;
		int * respuesta;
    		printf("¿Cuántos operandos desea ingresar? (máx 4): ");
    		if (scanf("%d", &cantidad) != 1 || cantidad < 1 || cantidad > 4) {
        		printf("Cantidad inválida. Debe estar entre 1 y 4.\n");
        		exit(1);
    		}

    		for (int i = 0; i < cantidad; i++) {
        		printf("Ingrese operando %d: ", i + 1);
        		if (scanf("%d", &valor) != 1) {
            			printf("Error leyendo operando.\n");
            			exit(1);
        		}

        	switch (i) {
            		case 0: parametros.operando1 = valor; break;
            		case 1: parametros.operando2 = valor; break;
            		case 2: parametros.operando3 = valor; break;
            		case 3: parametros.operando4 = valor; break;
        		}
		}
		gettimeofday(&inicio, NULL);
		respuesta = multiplicar_1(&parametros , clnt);
		gettimeofday(&fin, NULL);
		if(respuesta == NULL) {
			printf("Error al llamar al servicio multiplicar_1\n");
			exit(1);
		}
		printf("El resultado es: %i \n", *respuesta);
	}
	if(numero_servicio ==  5){
                suma_param parametros = {0,0,0,0};  // Inicializa todo a 0
                int cantidad, valor;
                int * respuesta;
                printf("¿Cuántos operandos desea ingresar? (máx 4): ");
                if (scanf("%d", &cantidad) != 1 || cantidad < 1 || cantidad > 4) {
                        printf("Cantidad inválida. Debe estar entre 1 y 4.\n");
                        exit(1);
                }

                for (int i = 0; i < cantidad; i++) {
                        printf("Ingrese operando %d: ", i + 1);
                        if (scanf("%d", &valor) != 1) {
                                printf("Error leyendo operando.\n");
                                exit(1);
                        }

                switch (i) {
                        case 0: parametros.operando1 = valor; break;
                        case 1: parametros.operando2 = valor; break;
                        case 2: parametros.operando3 = valor; break;
                        case 3: parametros.operando4 = valor; break;
                        }
                }
		gettimeofday(&inicio, NULL);
                respuesta = sumar_1(&parametros , clnt);
                gettimeofday(&fin, NULL);
		 if(respuesta == NULL) {
                        printf("Error al llamar al servicio sumar_1\n");
                        exit(1);
                }
                printf("El resultado es: %i \n", *respuesta);
        }
	long tiempo_microsegundos =  (fin.tv_sec - inicio.tv_sec) * 1000000L + (fin.tv_usec - inicio.tv_usec);
        printf("Tiempo de respuesta a la solicitud: %ld microsegundos\n", tiempo_microsegundos);	
	clnt_destroy(clnt);
    	return 0;
}
