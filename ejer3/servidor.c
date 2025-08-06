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

#define myport 666
#define backlog 66

int main (){
	int sockfd;
	int newfd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;
	int resultado_respuesta;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		printf("Error al crear el socket");
		exit(1);
	}
	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(myport);
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
			/*ERROR*/
		}
		printf("server: conexion desde: %s desde puerto %d \n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port));
		if(!fork()){
			resultado_respuesta = send(newfd, "ok\n", 5, 0);
			if( resultado_respuesta  == -1) perror ("send");
			close (newfd);
			exit(0);
		}
		close(newfd);
		while(waitpid(-1, NULL, WNOHANG) > 0);
	}
}	
