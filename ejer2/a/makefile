#Variables
RPCGEN = rpcgen
CC = gcc
CLIENTE = cliente
SERVIDOR = servidor
DISTRO := $(shell . /etc/os-release && echo $$ID)

ifeq ($(DISTRO),fedora)
	OFLAGS = -ltirpc
	CFLAGS = -I/usr/include/tirpc
else
	OFLAGS =
	CFLAGS =
endif

#Archivos generados
HDR = ejercicio2.h
CLNT = ejercicio2_clnt.c
XDR = ejercicio2_xdr.c
SVC = ejercicio2_svc.c

#Reglas
all:	$(CLIENTE) $(SERVIDOR)

#Ejecutable cliente
$(CLIENTE):	ejercicio2_cliente.o ejercicio2_clnt.o ejercicio2_xdr.o 
	$(CC) -o $(CLIENTE) ejercicio2_cliente.o ejercicio2_clnt.o ejercicio2_xdr.o $(OFLAGS)

#Ejecutable servidor
$(SERVIDOR):	ejercicio2_server.o ejercicio2_svc.o ejercicio2_xdr.o
	$(CC) -o $(SERVIDOR) ejercicio2_server.o ejercicio2_svc.o ejercicio2_xdr.o $(OFLAGS)

#Generacion de .o
%.o:	%.c
	$(CC) -c $< $(CFLAGS)

#Agrego dependencia para que no me tire error
ejercicio2_cliente.o:	$(HDR)
ejercicio2_server.o:	$(HDR)

#Generar archivos desde el rpcgen
$(HDR):	ejercicio2.x
	$(RPCGEN) ejercicio2.x
$(CLNT): ejercicio2.x
	$(RPCGEN) ejercicio2.x
$(XDR):	ejercicio2.x
	$(RPCGEN) ejercicio2.x
$(SVC):	ejercicio2.x
	$(RPCGEN) ejercicio2.x

#Limpiar archivos generados
clean:
	rm -f *.o $(CLIENTE) $(SERVIDOR) $(HDR) $(CLNT) $(XDR) $(SVC)
