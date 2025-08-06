#Variables
CC = gcc
CLIENTE = cliente
SERVIDOR = servidor
RPCGEN = rpcgen
DISTRO := $(shell . /etc/os-release && echo $$ID)

ifeq ($(DISTRO), fedora)
	OFLAGS = -ltirpc
	CFLAGS = -I/usr/include/tirpc
else
	OFLAGS =
	CFLAGS =
endif

#archivos generados por rpcgen
HDR = mini_servicios.h
CLNT = mini_servicios_clnt.c
XDR = mini_servicios_xdr.c
SVC = mini_servicios_svc.c

#regla principal
all: $(CLIENTE) $(SERVIDOR)

#ejecutable cliente
$(CLIENTE):	proc_cliente.o mini_servicios_clnt.o mini_servicios_xdr.o
	$(CC) -o $(CLIENTE) proc_cliente.o mini_servicios_clnt.o mini_servicios_xdr.o $(OFLAGS)

#Ejecutable servidor
$(SERVIDOR):	proc_servidor.o mini_servicios_svc.o mini_servicios_xdr.o
	$(CC) -o $(SERVIDOR) proc_servidor.o mini_servicios_svc.o mini_servicios_xdr.o $(OFLAGS)

#conpilar cualquier .c a .o
%.o: %.c
	$(CC) -c $< $(CFLAGS)

#dependencias de los .o con el .h generado
proc_cliente.o: $(HDR)
proc_servidor.o: $(HDR)

#generacion archivos rpc
$(HDR) $(CLNT) $(XDR) $(SVC): mini_servicios.x
	$(RPCGEN) mini_servicios.x

#limpiar archivos generados
clean:
	rm -f *.o $(CLIENTE) $(SERVIDOR) $(HDR) $(CLNT) $(XDR) $(SVC)
