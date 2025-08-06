#Variables
RPCGEN = rpcgen
CC= gcc
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

#Archivos generados por rpcgen
HDR = ejercicio1.h
CLNT = ejercicio1_clnt.c
XDR = ejercicio1_xdr.c
SVC = ejercicio1_svc.c

#Regla principal
all: $(CLIENTE) $(SERVIDOR)

#Compilar la tarea
tarea.o: tarea.c
	$(CC) -c tarea.c

tarea: tarea.o
	$(CC) -o tarea tarea.o

#Ejecutable cliente
$(CLIENTE):	ejercicio1_cliente.o ejercicio1_clnt.o ejercicio1_xdr.o
	$(CC) -o $(CLIENTE) ejercicio1_cliente.o ejercicio1_clnt.o ejercicio1_xdr.o $(OFLAGS)

#Ejecutable servidor
$(SERVIDOR):	ejercicio1_server.o ejercicio1_svc.o ejercicio1_xdr.o
	$(CC) -o $(SERVIDOR) ejercicio1_server.o ejercicio1_svc.o ejercicio1_xdr.o $(OFLAGS)

#Compilar cualquier .c a .o
%.o: %.c
	$(CC) -c $< $(CFLAGS)

#Dependencias de los .o con el .h generado
ejercicio1_cliente.o: $(HDR)
ejercicio1_server.o: $(HDR)

#Generar archivos RPC
$(HDR): ejercicio1.x
	$(RPCGEN) ejercicio1.x
$(CLNT): ejercicio1.x
	$(RPCGEN) ejercicio1.x
$(XDR): ejercicio1.x
	$(RPCGEN) ejercicio1.x
$(SVC): ejercicio1.x
	$(RPCGEN) ejercicio1.x

#Limpiar archivos generados
clean:
	rm -f *.o $(CLIENTE) $(SERVIDOR) $(HDR) $(CLNT) $(XDR) $(SVC) tarea
