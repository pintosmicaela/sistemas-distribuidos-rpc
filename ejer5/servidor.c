#include <stdio.h>              // printf, perror
#include <stdlib.h>             // exit
#include <string.h>             // bzero, memcpy
#include <unistd.h>             // close
#include <sys/types.h>          // tipos de sockets
#include <sys/socket.h>         // socket, connect
#include <netinet/in.h>         // struct sockaddr_in
#include <netdb.h>              // gethostbyname
#include <sys/wait.h>
#include <arpa/inet.h> 
#include <time.h>
#include "estructuras.h"

void solicitar_turno_matrimonio();
int solicitar_turno_nacimiento(struct turnos_registro_nacimiento * turno);
int solicitar_turno_patente_auto(struct turno_patentar_auto * turno);
char * solicitar_fecha_nueva();
void analizar_solicitud(char *buffer, char *resultados, struct turnos_registro_nacimiento *tn);
void cargar_informacion(struct informacion_nacimiento * info);
void cargar_informacion_patentes(struct informacion_patentes *info);
int buscar_informacion (struct informacion_nacimiento *info, char * dni);
int buscar_informacion_patente(struct informacion_patentes *info, char *pat);
const struct tm* TodayPlusDays(int days);
int crear_turno_matrimonio(struct solicitud_turno_matrimonio stm, struct turno_matrimonio_registrado *tmr);
int crear_turno_transferencia_auto(struct solicitud_transferencia_vehiculo stm, struct turno_transferencia_auto *tmr);
void mostrar_info_turno(char * fecha_nueva , int numero_turno);
void mostrar_info_turno_matrimonio( struct turno_matrimonio_registrado turno);
void mostrar_info_turno_transferencia_auto( struct turno_transferencia_auto turno);
void escuchar_turno_bebe();
void escuchar_turno_matrimonio();
void escuchar_obtener_informacion();
void escuchar_obtener_turno_patentar_auto();
void escuchar_obtener_turno_transferencia_auto();
void escuchar_obtener_informacion_patente();

int main (void){
        int status;
        if(!fork())
                escuchar_turno_matrimonio();
        if(!fork())
                escuchar_turno_bebe();
        if(!fork())
                escuchar_obtener_informacion();
        if(!fork())
                escuchar_obtener_turno_patentar_auto();
        if(!fork())
                escuchar_obtener_turno_transferencia_auto();
        if(!fork())
				escuchar_obtener_informacion_patente();
		wait(&status);
		wait(&status);
		wait(&status);
		wait(&status);
		wait(&status);
		wait(&status);
}
void escuchar_obtener_turno_transferencia_auto(){
        int sockfd, newfd,sin_size, resultado_respuesta;;
        struct sockaddr_in my_addr, their_addr;
        char buffer_solicitud[MAX_BUFFER];
        char resultados [MAX_BUFFER];
        int bytes_recibidos;
        struct solicitud_transferencia_vehiculo turno_transferencia;
        struct turno_transferencia_auto turnos_registrados[MAX_INFO];
        int i;
        //Inicializo la informacion de turnos con -1
        for(i=0; i<MAX_INFO;i++){
			turnos_registrados[i].turno = -1;
		}
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
                printf("Error al crear el socket");
                exit(1);
        }
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(puerto_turno_transferencia_auto);
        my_addr.sin_addr.s_addr = INADDR_ANY; /*IPLOCAL*/
        bzero(&(my_addr.sin_zero),8); /*rellena con ceros la estructura*/
        if ( bind( sockfd, (struct sockaddr *)&my_addr, sizeof( struct sockaddr)) == -1){
                printf("Error al crear el socket");
                exit(1);
        }
        if( listen(sockfd,backlog) == -1){
                printf("Error al convertir el puerto en pasivo");
                exit(1);
        }
        while(1){
                sin_size = sizeof(struct sockaddr_in);
                if ((newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) ==-1 ){
                        printf("Error al aceptar la conexion socket");
                        exit(1);
                }
                bytes_recibidos = recv (newfd, &turno_transferencia, sizeof(turno_transferencia), 0);
				if (bytes_recibidos == 0){
						printf("No se recibio informacion del cliente en la solicitud de matrimonio\n");
						exit(1);
				}
				int indice = crear_turno_transferencia_auto(turno_transferencia, turnos_registrados);
				resultado_respuesta = send(newfd, &turnos_registrados[indice] , sizeof(struct turno_transferencia_auto), 0);
				mostrar_info_turno_transferencia_auto(turnos_registrados[indice]);
				close (newfd);
        }
}

void mostrar_info_turno_transferencia_auto( struct turno_transferencia_auto turno){
	printf("---------------\n");
	printf("Turno de transferencia de auto solicitado\n");
	printf("nombre titular: %s\n", turno.nombre_duenio);
	printf("patente: %s\n", turno.patente);
	printf("fecha: %s\n", turno.fecha);
	printf("numero de turno: %i\n", turno.turno);
	printf("--------------\n");
}

int crear_turno_transferencia_auto(struct solicitud_transferencia_vehiculo stm, struct turno_transferencia_auto *tmr){
	  int indice = -1;
	  char * fecha_nueva = solicitar_fecha_nueva();
	  int i = 0;
	  while (indice == -1 && i < MAX_INFO){
		  if (tmr[i].turno ==-1){
			  indice = i;
			  strcpy(tmr[i].nombre_duenio, stm.nombre_duenio);
			  strcpy(tmr[i].patente, stm.patente);
			  strcpy(tmr[i].fecha, fecha_nueva);
			  tmr[i].turno = i+1;
		  }
		  i++;
	  }
	  return indice;
}

void escuchar_obtener_turno_patentar_auto(){
        struct turno_patentar_auto info_turnos_patentes[MAX_INFO];
        int sockfd, newfd,sin_size, resultado_respuesta, bytes_recibidos, resultado_solicitud;
        struct sockaddr_in my_addr, their_addr;
        struct turno_patentar_auto turno_obtenido;
        //Inicializo la informacion en "vacio"
        for (int i = 0; i < MAX_INFO; i++) {
                info_turnos_patentes[i].numero_turno = -1;
        }
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
                printf("Error al crear el socket");
                exit(1);
        }
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(puerto_turno_patente_auto);
        my_addr.sin_addr.s_addr = INADDR_ANY; /*IPLOCAL*/
        bzero(&(my_addr.sin_zero),8); /*rellena con ceros la estructura*/
        if ( bind( sockfd, (struct sockaddr *)&my_addr, sizeof( struct sockaddr)) == -1){
                printf("Error al crear el socket"),
                exit(1);
        }
        if( listen(sockfd,backlog) == -1){
                printf("Error al convertir el puerto en pasivo");
                exit(1);
        }
        while(1){
                sin_size = sizeof(struct sockaddr_in);
                char rec[4];
                if ((newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) ==-1 ){
                        printf("Error al aceptar la conexion socket");
                        exit(1);
                }
				bytes_recibidos = recv (newfd, &rec, 4, 0);
				if (bytes_recibidos == 0){
						printf("No se recibio informacion del cliente en la solicitud de turno de patente de auto\n");
						exit(1);
				} 
				resultado_solicitud = solicitar_turno_patente_auto(info_turnos_patentes);
				resultado_respuesta = send(newfd, &info_turnos_patentes[resultado_solicitud] , sizeof(struct turno_patentar_auto), 0);
				if( resultado_respuesta  == -1) printf ("Error al enviar informacion al cliente");
                close(newfd);
        }
}

int solicitar_turno_patente_auto(struct turno_patentar_auto *tn){
        char * fecha_nueva = solicitar_fecha_nueva();
        int i = 0;
		int terminado = 0;
        while(  terminado == 0 && i < MAX_INFO){
                if(tn[i].numero_turno == -1){
                    strcpy(tn[i].fecha, fecha_nueva);
					tn[i].numero_turno = i+1;
					mostrar_info_turno(fecha_nueva, i);
                    terminado = 1;
                }
                i++;
        }
        return i -1;
}

void escuchar_obtener_informacion_patente(){
	struct informacion_patentes info_patentes[MAX_INFO];
	int sockfd, newfd,sin_size, resultado_respuesta, bytes_recibidos, resultado_solicitud;
    struct sockaddr_in my_addr, their_addr;
    cargar_informacion_patentes(info_patentes);
    char patente_solicitada[TAM_FECHA];
     if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
                printf("Error al crear el socket");
                exit(1);
        }
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(puerto_informacion_patentes);
        my_addr.sin_addr.s_addr = INADDR_ANY; /*IPLOCAL*/
        bzero(&(my_addr.sin_zero),8); /*rellena con ceros la estructura*/
        if ( bind( sockfd, (struct sockaddr *)&my_addr, sizeof( struct sockaddr)) == -1){
                printf("Error al crear el socket"),
                exit(1);
        }
        if( listen(sockfd,backlog) == -1){
                printf("Error al convertir el puerto en pasivo");
                exit(1);
        }
        while(1){
			 sin_size = sizeof(struct sockaddr_in);
             if ((newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) ==-1 ){
				printf("Error al aceptar la conexion socket");
				exit(1);
             }
             if(!fork()){
				bytes_recibidos = recv (newfd, &patente_solicitada, sizeof(patente_solicitada), 0);
				if (bytes_recibidos == 0){
					printf("No se recibio informacion del cliente en la solicitud de patente\n");
					exit(1);
				} 
				int indice = buscar_informacion_patente(info_patentes, patente_solicitada);
				if (indice == -1)
					printf("No se encontra la informacion que solicita\n");
				else {
					printf("------------------------\n");
					printf("El nombre es: %s\nLa patente es: %s\nEl modelo es: %s\nEl numero de chasis es: %s\n", info_patentes[indice].nombre, info_patentes[indice].patente, info_patentes[indice].modelo, info_patentes[indice].nro_chasis);
					printf("------------------------\n");
				}
				resultado_respuesta = send(newfd, &info_patentes[indice] , sizeof(struct informacion_patentes), 0);
				if( resultado_respuesta  == -1) perror ("send");
					close (newfd);
					exit(0);
                }
			close(newfd);
			while(waitpid(-1, NULL, WNOHANG) > 0);
        }
}

void escuchar_turno_bebe(){
        struct turnos_registro_nacimiento info_turnos_nacimientos[MAX_INFO];
        int sockfd, newfd,sin_size, resultado_respuesta, bytes_recibidos, resultado_solicitud;
        struct sockaddr_in my_addr, their_addr;
        struct turnos_registro_nacimiento turno_registro_nacimiento;
        //Inicializo la informacion en "vacio"
        for (int i = 0; i < MAX_INFO; i++) {
                info_turnos_nacimientos[i].numero_turno = -1;
        }
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
                printf("Error al crear el socket");
                exit(1);
        }
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(puerto_turno_nacimiento);
        my_addr.sin_addr.s_addr = INADDR_ANY; /*IPLOCAL*/
        bzero(&(my_addr.sin_zero),8); /*rellena con ceros la estructura*/
        if ( bind( sockfd, (struct sockaddr *)&my_addr, sizeof( struct sockaddr)) == -1){
                printf("Error al crear el socket"),
                exit(1);
        }
        if( listen(sockfd,backlog) == -1){
                printf("Error al convertir el puerto en pasivo");
                exit(1);
        }
        while(1){
                sin_size = sizeof(struct sockaddr_in);
                char rec[4];
                if ((newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) ==-1 ){
                        printf("Error al aceptar la conexion socket");
                        exit(1);
                }
				bytes_recibidos = recv (newfd, &rec, 4, 0);
				if (bytes_recibidos == 0){
						printf("No se recibio informacion del cliente en la solicitud de turno bebe\n");
						exit(1);
				} 
				resultado_solicitud = solicitar_turno_nacimiento(info_turnos_nacimientos);
				resultado_respuesta = send(newfd, &info_turnos_nacimientos[resultado_solicitud] , sizeof(struct turnos_registro_nacimiento), 0);
				if( resultado_respuesta  == -1) printf ("Error al enviar informacion al cliente");
                close(newfd);
        }
}

void escuchar_turno_matrimonio(){
        int sockfd, newfd,sin_size, resultado_respuesta;;
        struct sockaddr_in my_addr, their_addr;
        char buffer_solicitud[MAX_BUFFER];
        char resultados [MAX_BUFFER];
        int bytes_recibidos;
        struct solicitud_turno_matrimonio turno_matrimonio;
        struct turno_matrimonio_registrado turnos_registrados[MAX_INFO];
        int i;
        //Inicializo la informacion de turnos con -1
        for(i=0; i<MAX_INFO;i++){
			turnos_registrados[i].turno = -1;
		}
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
                printf("Error al crear el socket");
                exit(1);
        }
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(puerto_turno_matrimonio);
        my_addr.sin_addr.s_addr = INADDR_ANY; /*IPLOCAL*/
        bzero(&(my_addr.sin_zero),8); /*rellena con ceros la estructura*/
        if ( bind( sockfd, (struct sockaddr *)&my_addr, sizeof( struct sockaddr)) == -1){
                printf("Error al crear el socket");
                exit(1);
        }
        if( listen(sockfd,backlog) == -1){
                printf("Error al convertir el puerto en pasivo");
                exit(1);
        }
        while(1){
                sin_size = sizeof(struct sockaddr_in);
                if ((newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) ==-1 ){
                        printf("Error al aceptar la conexion socket");
                        exit(1);
                }
                bytes_recibidos = recv (newfd, &turno_matrimonio, sizeof(turno_matrimonio), 0);
				if (bytes_recibidos == 0){
						printf("No se recibio informacion del cliente en la solicitud de matrimonio\n");
						exit(1);
				}
				int indice = crear_turno_matrimonio(turno_matrimonio, turnos_registrados);
				resultado_respuesta = send(newfd, &turnos_registrados[indice] , sizeof(struct turno_matrimonio_registrado), 0);
				mostrar_info_turno_matrimonio(turnos_registrados[indice]);
				close (newfd);
        }
}
void escuchar_obtener_informacion(){
        int sockfd, newfd,sin_size, resultado_respuesta, bytes_recibidos;
        struct sockaddr_in my_addr, their_addr;
        char buffer_solicitud[MAX_BUFFER];
        char resultados [MAX_BUFFER];
        struct informacion_nacimiento informacion[MAX_INFO];
        cargar_informacion(informacion);
        char dni[TAM_FECHA];
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
                printf("Error al crear el socket");
                exit(1);
        }
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(puerto_informacion_nacimiento);
        my_addr.sin_addr.s_addr = INADDR_ANY; /*IPLOCAL*/
        bzero(&(my_addr.sin_zero),8); /*rellena con ceros la estructura*/
        if ( bind( sockfd, (struct sockaddr *)&my_addr, sizeof( struct sockaddr)) == -1){
                printf("Error al crear el socket"),
                exit(1);
        }
        if( listen(sockfd,backlog) == -1){
                printf("Error al convertir el puerto en pasivo");
                exit(1);
        }
        while(1){
                sin_size = sizeof(struct sockaddr_in);
                if ((newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) ==-1 ){
                        printf("Error al aceptar la conexion socket");
                        exit(1);
                }
                if(!fork()){
                        bytes_recibidos = recv (newfd, &dni, sizeof(dni), 0);
                        if (bytes_recibidos == 0){
                                printf("No se recibio informacion del cliente en la solicitud de matrimonio\n");
                                exit(1);
                        } 
                        int indice = buscar_informacion(informacion, dni);
                        if (indice == -1)
                                 printf("No se encontra la informacion que solicita\n");
                        else {
								printf("------------------------\n");
                                printf("El nombre es: %s\nLa fecha de nacimiento es: %s\nEl sexo es: %c\nEl dni es: %s\n", informacion[indice].nombre, informacion[indice].fecha, informacion[indice].sexo, informacion[indice].documento);
								printf("------------------------\n");
							}
                        resultado_respuesta = send(newfd, &informacion[indice] , sizeof(struct informacion_nacimiento), 0);
                        if( resultado_respuesta  == -1) perror ("send");
                        close (newfd);
                        exit(0);
                }
                close(newfd);
                while(waitpid(-1, NULL, WNOHANG) > 0);
        }


}
void mostrar_info_turno_matrimonio( struct turno_matrimonio_registrado turno){
	printf("---------------\n");
	printf("Turno de matrimonio concedido\n");
	printf("nombre persona 1: %s\n", turno.nombre1);
	printf("nombre persona 2: %s\n", turno.nombre2);
	printf("fecha: %s\n", turno.fecha);
	printf("numero de turno: %i\n", turno.turno);
	printf("--------------\n");
}
int solicitar_turno_nacimiento(struct turnos_registro_nacimiento *tn){
        char * fecha_nueva = solicitar_fecha_nueva();
        int i = 0;
		int terminado = 0;
        while(  terminado == 0 && i < MAX_INFO){
                if(tn[i].numero_turno == -1){
                    strcpy(tn[i].fecha, fecha_nueva);
					tn[i].numero_turno = i+1;
					mostrar_info_turno(fecha_nueva, i);
                    terminado = 1;
                }
                i++;
        }
        return i -1;
}

void mostrar_info_turno(char * fecha_nueva , int numero_turno){
	printf("----------------------------------------------\n");
	printf("Se realizo una reserva para un turno para un bebe recien nacido.\nFecha:%s\nNumero de turno:%i\n", fecha_nueva,numero_turno);
	printf("----------------------------------------------\n");
}
// Devuelve un puntero a un string con la fecha 7 días después de hoy
char * solicitar_fecha_nueva() {
    struct tm today = *TodayPlusDays(7);
    char * buffer = malloc(sizeof(char) * TAM_FECHA);
	sprintf(buffer, "%02d-%02d-%04d", today.tm_mday, today.tm_mon + 1, today.tm_year + 1900);
    return buffer;
}

const struct tm* TodayPlusDays( int days ){
    const time_t ONE_DAY = 24 * 60 * 60 ;
    time_t date_seconds = time( NULL ) + (days * ONE_DAY) ;
    return localtime( &date_seconds ) ;
}

void cargar_informacion(struct informacion_nacimiento * info){
  strcpy(info[0].nombre,"Sofia");
  strcpy(info[0].fecha,"12-01-1990");
  info[0].sexo = 'F';
  strcpy(info[0].documento,"123445");
  strcpy(info[1].nombre,"Tomas");
  strcpy(info[1].fecha,"08-11-1998");
  info[1].sexo = 'M';
  strcpy(info[1].documento,"124536");
  strcpy(info[2].nombre,"Lucas");
  strcpy(info[2].fecha,"19-09-2002");
  info[2].sexo = 'M';
  strcpy(info[2].documento,"454321");
  strcpy(info[3].nombre,"Martina");
  strcpy(info[3].fecha,"01-09-2003");
  info[3].sexo = 'F';
  strcpy(info[3].documento,"567567");
  strcpy(info[4].nombre,"Agustin");
  strcpy(info[4].fecha,"27-05-1994");
  info[4].sexo = 'M';
  strcpy(info[4].documento,"112334");
  strcpy(info[5].nombre,"Julieta");
  strcpy(info[5].fecha,"22-12-1995");
  info[5].sexo = 'F';
  strcpy(info[5].documento,"124567");
  strcpy(info[6].nombre,"Mateo");
  strcpy(info[6].fecha,"03-10-1988");
  info[6].sexo = 'M';
  strcpy(info[6].documento,"115678");
  strcpy(info[7].nombre,"Valentina");
  strcpy(info[7].fecha,"14-06-1997");
  info[7].sexo = 'F';
  strcpy(info[7].documento,"129087");
  strcpy(info[8].nombre,"Nicolas");
  strcpy(info[8].fecha,"30-01-2000");
  info[8].sexo = 'M';
  strcpy(info[8].documento,"267231");
  strcpy(info[9].nombre,"Camila");
  strcpy(info[9].fecha,"08-11-1993");
  info[9].sexo = 'F';
  strcpy(info[9].documento,"123987");
}

int buscar_informacion (struct informacion_nacimiento *info, char * dni){
  int i = 0;
  int esta = -1;
  while ( i < MAX_INFO){
    if(strcmp(info[i].documento, dni) == 0){
      esta = i;
      break;
    }
    i++;
  }
  return esta;
  }
 int buscar_informacion_patente(struct informacion_patentes *info, char *pat){
	int i = 0;
	int esta = -1;
	while ( i < MAX_INFO){
    if(strcmp(info[i].patente, pat) == 0){
      esta = i;
      break;
    }
    i++;
  }
  return esta;
}
int crear_turno_matrimonio(struct solicitud_turno_matrimonio stm, struct turno_matrimonio_registrado *tmr){
	  int indice = -1;
	  char * fecha_nueva = solicitar_fecha_nueva();
	  int i = 0;
	  while (indice == -1 && i < MAX_INFO){
		  if (tmr[i].turno ==-1){
			  indice = i;
			  strcpy(tmr[i].nombre1, stm.nombre1);
			  strcpy(tmr[i].nombre2, stm.nombre2);
			  strcpy(tmr[i].fecha, fecha_nueva);
			  tmr[i].turno = i;
		  }
		  i++;
	  }
	  return indice;
}

void cargar_informacion_patentes(struct informacion_patentes *info){
	strcpy(info[0].nombre, "Juan");
	strcpy(info[0].patente, "ABC123");
	strcpy(info[0].modelo, "Toyota");
	strcpy(info[0].nro_chasis, "JTDBR3");
	strcpy(info[1].nombre, "Maria");
	strcpy(info[1].patente, "ZXY456");
	strcpy(info[1].modelo, "Ford");
	strcpy(info[1].nro_chasis, "2FADP4");
	strcpy(info[2].nombre, "Luis");
	strcpy(info[2].patente, "QWE789");
	strcpy(info[2].modelo, "Chevrolet");
	strcpy(info[2].nro_chasis, "9BGKS4");
	strcpy(info[3].nombre, "Ana");
	strcpy(info[3].patente, "MNO321");
	strcpy(info[3].modelo, "Renault");
	strcpy(info[3].nro_chasis, "8A1BBH");
	strcpy(info[4].nombre, "Diego");
	strcpy(info[4].patente, "TUV654");
	strcpy(info[4].modelo, "Volkswagen");
	strcpy(info[4].nro_chasis, "93HBX4");
	strcpy(info[5].nombre, "Laura");
	strcpy(info[5].patente, "HJK987");
	strcpy(info[5].modelo, "Peugeot");
	strcpy(info[5].nro_chasis, "VF3CC8");
	strcpy(info[6].nombre, "Santiago");
	strcpy(info[6].patente, "PLM258");
	strcpy(info[6].modelo, "Nissan");
	strcpy(info[6].nro_chasis, "3N1CN7");
	strcpy(info[7].nombre, "Carla");
	strcpy(info[7].patente, "RST963");
	strcpy(info[7].modelo, "Honda");
	strcpy(info[7].nro_chasis, "2HGFC2");
	strcpy(info[8].nombre, "Federico");
	strcpy(info[8].patente, "BVC741");
	strcpy(info[8].modelo, "Fiat");
	strcpy(info[8].nro_chasis, "8APX1B");
	strcpy(info[9].nombre, "Valentina");
	strcpy(info[9].patente, "NMB159");
	strcpy(info[9].modelo, "Kia");
	strcpy(info[9].nro_chasis, "KNADM4");
}
