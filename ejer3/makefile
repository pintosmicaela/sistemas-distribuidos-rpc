#Variables 
CC = gcc
CLIENTE = cliente
SERVIDOR = servidor

all: $(CLIENTE) $(SERVIDOR)

$(CLIENTE): cliente.o
	$(CC) -o cliente cliente.c

$(SERVIDOR): servidor.o
	$(CC) -o servidor servidor.c
clean:
	rm -f *.o $(CLIENTE) $(SERVIDOR)
