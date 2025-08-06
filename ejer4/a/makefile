#Variables 
CC = gcc
SERVICIOS = servicios

all: $(SERVICIOS)

$(SERVICIOS): mini-servicios.o
	$(CC) -o servicios mini-servicios.c

clean:
	rm -f *.o $(SERVICIOS) 
