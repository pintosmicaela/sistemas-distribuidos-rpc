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

long long obtener_desvio_servidor(char * host){
	
	struct sockaddr_in their_addr; //direccion del server
	struct hostent * direccion_servidor; //Se utiliza para convertir el nombre del host a su ip
	int TAM_SOLICITUD = sizeof(double);
	long long T0 , T3, desvio;
	long long respuesta; 
	int sockfd, numbytes;
	direccion_servidor = gethostbyname(host);
	if(direccion_servidor == NULL){
		printf("Error, hostname %s incorrecto", host);
		exit(1);
	}
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
	close(sockfd);
	return diferencia;
}

int main(int argc, char * argv[]){
	if(argc !=4){
		printf("Error en la cantidad de argumentos\n");
		printf("formato correcto: cliente <ip.servidor1> <ip.servidor2> <ip.servidor3> ");
		exit(1);
	}
	long long desvio1, desvio2, desvio3;
	desvio1 = obtener_desvio_servidor(argv[1]);
	desvio2 = obtener_desvio_servidor(argv[2]);
	desvio3 = obtener_desvio_servidor(argv[3]);
	printf("\nDesvio primer servidor: %lld\nDesvio segundo servidor: %lld\nDesvio tercer servidor: %lld\n", desvio1, desvio2, desvio3);
	long long desvio_resultado = (desvio1 + desvio2 + desvio3) / 3;
	printf("El desvio del cliente es igual a: %lld\n", desvio_resultado);
	return 0; 
}


