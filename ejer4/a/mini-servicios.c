#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/time.h>

#define OP_NOMBRE 1
#define OP_FECHA 2
#define OP_BINARIO 3
#define OP_SUMA 4
#define OP_MULT 5

struct Mensaje {
    int operacion;
    int operandos[4];
    int cantidad_operandos;
    int resultado;
    char resultado_str[128];
    int solicitud_nueva;
    int respuesta_lista;
};

void convertir_binario(int num, char *dest) {
    char buffer[33];
    int i = 0;
    while (num > 0 && i < 32) {
        buffer[i++] = (num % 2) + '0';
        num = num / 2;
    }
    buffer[i] = '\0';
    int len = (i < 33 - 1) ? i : 33 - 1;
    for (int j = 0; j < len; j++) {
        dest[j] = buffer[i - j - 1];
    }
    dest[len] = '\0';
}

int main(void) {
    key_t key = ftok("mini-servicios.c", 65); 
    int shmid = shmget(key, sizeof(struct Mensaje), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    struct timeval inicio, fin;
    struct Mensaje *mem = (struct Mensaje *) shmat(shmid, NULL, 0);
    pid_t pid = fork();
    if (pid == 0) {
        while (1) {
            if (mem->solicitud_nueva) {
                // Procesar solicitud
                switch (mem->operacion) {
                    case OP_NOMBRE:
						gettimeofday(&inicio, NULL);
                        gethostname(mem->resultado_str, sizeof(mem->resultado_str));
                        gettimeofday(&fin, NULL);
                        break;
                    case OP_FECHA: {
						gettimeofday(&inicio, NULL);
                        time_t t = time(NULL);
                        snprintf(mem->resultado_str, sizeof(mem->resultado_str), "%s", ctime(&t));
                        gettimeofday(&fin, NULL);
                        break;
                    }
                    case OP_BINARIO:
						gettimeofday(&inicio, NULL);
                        convertir_binario(mem->operandos[0], mem->resultado_str);
                        gettimeofday(&fin, NULL);
                        break;
                    case OP_SUMA: {
						gettimeofday(&inicio, NULL);
                        int suma = 0;
                        for (int i = 0; i < mem->cantidad_operandos; i++)
                            suma += mem->operandos[i];
                        mem->resultado = suma;
                        gettimeofday(&fin, NULL);
                        break;
                    }
                    case OP_MULT: {
						gettimeofday(&inicio, NULL);
                        int mult = 1;
                        for (int i = 0; i < mem->cantidad_operandos; i++)
                            mult *= mem->operandos[i];
                        mem->resultado = mult;
                        gettimeofday(&fin, NULL);
                        break;
                    }
                    default:
                        strcpy(mem->resultado_str, "Operación inválida.");
                }
                long tiempo_microsegundos = (fin.tv_sec - inicio.tv_sec) * 1000000L + (fin.tv_usec - inicio.tv_usec);
				printf("Tiempo de respuesta a la solicitud: %ld microsegundos\n", tiempo_microsegundos);

                mem->respuesta_lista = 1;
                mem->solicitud_nueva = 0;
            }
            sleep(1);
        }

    } else {
        while (1) {
            printf("Operaciones de mini-servicios:\n");
            printf("1. Nombre de la maquina\n");
            printf("2. Fecha del sistema\n");
            printf("3. Decimal a Binario\n");
            printf("4. Suma (hasta 4 operandos)\n");
            printf("5. Multiplicación (hasta 4 operandos)\n");
            printf("0. Salir\n");
            printf("Seleccione operación: ");

            int operac;
            scanf("%d", &operac);
            if (operac == 0) break;
            mem->operacion = operac;
            mem->cantidad_operandos = 0;
            mem->resultado = 0;
            mem->respuesta_lista = 0;
			
            if (operac == OP_SUMA || operac == OP_MULT) {
                printf("Cantidad de operandos (1-4): ");
                scanf("%d", &mem->cantidad_operandos);
                for (int i = 0; i < mem->cantidad_operandos; i++) {
                    printf("Operando %d: ", i + 1);
                    scanf("%d", &mem->operandos[i]);
                }
            } else if (operac == OP_BINARIO) {
                mem->cantidad_operandos = 1;
                printf("Número a convertir: ");
                scanf("%d", &mem->operandos[0]);
            }

            mem->solicitud_nueva = 1;

            // Esperar la respuesta
            while (!mem->respuesta_lista) {
                sleep(1);
            }
            if (operac == OP_SUMA || operac == OP_MULT) {
                printf("Resultado: %d\n\n", mem->resultado);
            } else if(operac == OP_FECHA){
					printf("Resultado: %s\n", mem->resultado_str);
				} else{
					printf("Resultado: %s\n\n", mem->resultado_str);
            }
            mem->respuesta_lista = 0;
        }
        shmdt(mem);
        shmctl(shmid, IPC_RMID, NULL);
        printf("Cerrando mini-servicios.\n");
    }
    return 0;
}
