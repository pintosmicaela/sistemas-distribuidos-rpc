#include <stdio.h>              // printf, perror
#include <stdlib.h>             // exit
#include <string.h>             // bzero, memcpy
#include <unistd.h>             // close
#include <sys/types.h>          // tipos de sockets
#include <sys/socket.h>         // socket, connect
#include <netinet/in.h>         // struct sockaddr_in
#include <netdb.h>              // gethostbyname
#include <sys/time.h>
#include "estructuras.h"


#define port 666
#define maxdatasize 100
#define TAM_SOLICITUD 4096
#define TAM_RESPUESTA 4
#define TAM_MAX_NOMBRES 50
#define TAM_DOCUMENTO 10
#define TAM_NOMBRE 50
#define TAM_FECHA 11

int main(int argc, char * argv[]){
        int sockfd, numbytes;
        char buf[maxdatasize]; //buffer donde se reciben los datos
        struct hostent * direccion_servidor; //Se utiliza para convertir el nombre del host a su ip
        struct sockaddr_in their_addr; //direccion del server
        int numero_solicitud;
        char nombre_persona1[TAM_MAX_NOMBRES];
        char nombre_persona2[TAM_MAX_NOMBRES];
        char buffer[TAM_SOLICITUD];      // Lo que se envía
        char respuesta[TAM_RESPUESTA]; // Lo que se recibe
		char dni [TAM_FECHA];
	char patente[TAM_FECHA];
        struct solicitud_turno_matrimonio solicitud_turno_matrimonio;
        struct solicitud_transferencia_vehiculo solicitud_trasferencia_auto;
        struct turnos_registro_nacimiento turno_nacimiento;
        struct informacion_nacimiento informacion_natalidad;
	struct turno_matrimonio_registrado turno_matrimonio;
        struct turno_transferencia_auto turno_transf;
        struct turno_patentar_auto turno_obtenido_patente_auto;
        struct informacion_patentes infos_patentes;
        int resultado_conexion;
        if(argc !=2){
                printf("Error en la cantidad de argumentos\n");
                printf("formato correcto: cliente <ip.servidor>");
                exit(1);
        }
        direccion_servidor = gethostbyname(argv[1]);
        if(direccion_servidor == NULL){
                printf("Error, hostname incorrecto");
                exit(1);
        }
        //crea el socket
        sockfd  = socket( AF_INET, SOCK_STREAM, 0 );
        if( sockfd == 0 ){
                printf("Error al crear el socket");
                exit(1);
        }



        printf ("1. Solicitar un turno para obtener la licencia de matrimonio.\n");
        printf ("2. Solicitar informacion sobre la partida de nacimiento.\n");
        printf ("3. Solicitar un turno para la inscripcion de nacimiento.\n");
        printf ("4. Solicitar un turno para patentar auto.\n");
        printf ("5. Solicitar turno para tranferencia de vehiculo\n");
        printf ("6. Solicitar informacion sobre alguna patente.\n");
        printf ("Ingrese el numero del servicio que desea realizar:\n");

        their_addr.sin_family = AF_INET; 
        their_addr.sin_addr = *((struct in_addr *)direccion_servidor->h_addr); 
        bzero(&(their_addr.sin_zero), 8);
        scanf("%i", &numero_solicitud);
        if (numero_solicitud == 6) {
            printf ("Ingrese la patente: ");
            scanf ("%s", patente);
            their_addr.sin_port = htons(puerto_informacion_patentes);
            resultado_conexion =  connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if ( resultado_conexion == -1){
                            printf("Error al conectar con el servidor");
                            exit(1);
                }
            send(sockfd, patente, sizeof(patente), 0);
            recv (sockfd, &infos_patentes, sizeof(infos_patentes), 0);
            printf("El nombre es: %s\nLa patente es: %s\nEl modelo es: %s\nEl numero de chasis es: %s\n", infos_patentes.nombre, infos_patentes.patente, infos_patentes.modelo, infos_patentes.nro_chasis);
		}
        if(numero_solicitud == 4){
            their_addr.sin_port = htons(puerto_turno_patente_auto);
            resultado_conexion =  connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if ( resultado_conexion == -1){
                    printf("Error al conectar con el servidor");
                    exit(1);
            }
            send(sockfd, "soli", 4, 0);
            recv (sockfd, &turno_obtenido_patente_auto, sizeof(turno_obtenido_patente_auto), 0);
            printf("----------------------------------------------\n");
            printf("Se obtuvo una reserva para un turno para patentar.\nFecha:%s\nNumero de turno:%i\n", turno_obtenido_patente_auto.fecha,turno_obtenido_patente_auto.numero_turno);
            printf("----------------------------------------------\n");
        }
        if(numero_solicitud == 5){
            their_addr.sin_port = htons(puerto_turno_transferencia_auto);
            resultado_conexion =  connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if ( resultado_conexion == -1){
                    printf("Error al conectar con el servidor");
                    exit(1);
            }
            printf ("Ingrese la patente del auto a transferir seguido del nombre del titular: ");
            scanf ("%s %s", solicitud_trasferencia_auto.patente, solicitud_trasferencia_auto.nombre_duenio);
            int resultado_envio = send(sockfd, &solicitud_trasferencia_auto, sizeof(solicitud_trasferencia_auto), 0);
            if (resultado_envio == -1) {
                perror("Error al enviar los datos");
                exit(1);
            }
            recv (sockfd, &turno_transf, sizeof(turno_transf), 0);
            printf("-----------------------------\n");
            printf("Turno para transferencia de auto obtenido.\nFecha: %s\n", turno_transf.fecha);
            printf("-----------------------------\n");
        }
        if (numero_solicitud == 1) {
            their_addr.sin_port = htons(puerto_turno_matrimonio);
            resultado_conexion =  connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if ( resultado_conexion == -1){
                    printf("Error al conectar con el servidor");
                    exit(1);
            }
            printf ("Ingrese el nombre de las personas: ");
            scanf ("%s %s", solicitud_turno_matrimonio.nombre1, solicitud_turno_matrimonio.nombre2);
            //printf("\nnombre1: %s\nnombre2: %s",turno_matrimonio.nombre1, turno_matrimonio.nombre2);
            int resultado_envio = send(sockfd, &solicitud_turno_matrimonio, sizeof(solicitud_turno_matrimonio), 0);
            if (resultado_envio == -1) {
                perror("Error al enviar los datos");
                exit(1);
            }
            recv (sockfd, &turno_matrimonio, sizeof(turno_matrimonio), 0);
            printf("-----------------------------\n");
            printf("Turno matrimonio obtenido.\nFecha: %s\n", turno_matrimonio.fecha);
            printf("-----------------------------\n");
        }
        if (numero_solicitud == 2) {
            printf ("Ingrese el documento: ");
            scanf ("%s", dni);
            their_addr.sin_port = htons(puerto_informacion_nacimiento);
            resultado_conexion =  connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if ( resultado_conexion == -1){
                            printf("Error al conectar con el servidor");
                            exit(1);
                }
            send(sockfd, dni, sizeof(dni), 0);
            recv (sockfd, &informacion_natalidad, sizeof(informacion_natalidad), 0);
            printf("El nombre es: %s\nLa fecha de nacimiento es: %s\nEl sexo es: %c\nEl dni es: %s\n", informacion_natalidad.nombre, informacion_natalidad.fecha, informacion_natalidad.sexo, informacion_natalidad.documento);
		}
		if (numero_solicitud == 3) {
            their_addr.sin_port = htons(puerto_turno_nacimiento);
            resultado_conexion =  connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if ( resultado_conexion == -1){
                    printf("Error al conectar con el servidor");
                    exit(1);
            }
            send(sockfd, "soli", 4, 0);
            recv (sockfd, &turno_nacimiento, sizeof(turno_nacimiento), 0);
            printf("----------------------------------------------\n");
            printf("Se obtuvo una reserva para un turno para un bebe recien nacido.\nFecha:%s\nNumero de turno:%i\n", turno_nacimiento.fecha,turno_nacimiento.numero_turno);
            printf("----------------------------------------------\n");
        }
		if (numero_solicitud > 6 ) printf ("Ingreso una opcion incorrecta.\n");
        close(sockfd); 
        return 0; 
} 
