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
#define TAM_SOLICITUD 4096
#define TAM_RESPUESTA 4
int main(int argc, char * argv[]){

	int sockfd, numbytes;
	char buf[maxdatasize]; //buffer donde se reciben los datos
	struct hostent * direccion_servidor; //Se utiliza para convertir el nombre del host a su ip
	struct sockaddr_in their_addr; //direccion del server
	struct timeval inicio, fin;	


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

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(port); 
	their_addr.sin_addr = *((struct in_addr *)direccion_servidor->h_addr); 
	bzero(&(their_addr.sin_zero), 8);
	
	int resultado_conexion =  connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));
	if ( resultado_conexion == -1){
		printf("Error al conectar con el servidor");
		exit(1);
	}

	char buffer[TAM_SOLICITUD];      // Lo que se envía
	char respuesta[TAM_RESPUESTA]; // Lo que se recibe
	
	gettimeofday(&inicio, NULL);
	send(sockfd, buffer, TAM_SOLICITUD, 0);
	recv(sockfd, respuesta, TAM_RESPUESTA, 0);
	gettimeofday(&fin, NULL);
	
	printf("recibido: %s\n",respuesta);

	long tiempo_microsegundos =  (fin.tv_sec - inicio.tv_sec) * 1000000L + (fin.tv_usec - inicio.tv_usec);
	printf("Tiempo de respuesta a la solicitud: %ld microsegundos\n", tiempo_microsegundos);
	
	close(sockfd); 
	return 0; 
} 

