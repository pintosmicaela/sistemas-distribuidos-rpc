#variables
CC = gcc
SERVIDOR = servidor
CLIENTE = cliente

all: $(CLIENTE) $(SERVIDOR)

$(CLIENTE):	cliente.o estructuras.h
	$(CC) -o $(CLIENTE) cliente.c estructuras.h

$(SERVIDOR):	servidor.o estructuras.h
	$(CC) -o $(SERVIDOR) servidor.c estructuras.h

clean:
	rm -f *.o $(CLIENTE) $(SERVIDOR)
