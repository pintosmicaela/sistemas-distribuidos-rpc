#include <stdio.h>              // printf, perror
#include <stdlib.h>             // exit
#include <string.h>             // bzero, memcpy
#include <unistd.h>             // close
#include <sys/types.h>          // tipos de sockets
#include <sys/socket.h>         // socket, connect
#include <netinet/in.h>         // struct sockaddr_in
#include <netdb.h>              // gethostbyname
#include <sys/time.h>

#define port 666
#define	maxdatasize 100

long long obtener_tiempo_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long resultado = tv.tv_sec * 1000000L + tv.tv_usec ;
    return resultado ;
}

int main(int argc, char * argv[]){
	int TAM_SOLICITUD = sizeof(double);
	int sockfd;
	char buf[maxdatasize]; //buffer donde se reciben los datos
	struct hostent * direccion_servidor; //Se utiliza para convertir el nombre del host a su ip
	struct sockaddr_in their_addr; //direccion del server
	long long T0 , T3, desvio;

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

	long long respuesta; 

	sockfd  = socket( AF_INET, SOCK_STREAM, 0 );
        if( sockfd == 0 ){
         	printf("Error al crear el socket");
                exit(1);
	}
	their_addr.sin_family = AF_INET;
        their_addr.sin_port = htons(port); 
        their_addr.sin_addr = *((struct in_addr *)direccion_servidor->h_addr); 
        bzero(&(their_addr.sin_zero), 8);
	int resultado_conexion =  connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));
        if ( resultado_conexion == -1){
                printf("Error al conectar con el servidor");
                exit(1);
        }		
	T0 = obtener_tiempo_ms();
	recv(sockfd, &respuesta, TAM_SOLICITUD, 0);
	T3 = obtener_tiempo_ms();
	long long diferencia =  T3 + T0;
	diferencia = respuesta-diferencia;
	printf("La diferencia en microsegundos con respecto al servidor es igual a: %d\n", diferencia);
		
	close(sockfd); 
	return 0; 
}
