#Variables
CC = gcc
TAREA = tarea
EJERCICIO = ejercicio1

all: $(TAREA) $(EJERCICIO)

%.o: %.c
	$(CC) -c $<

clean:
	rm -f *.o $(EJERCICIO) $(TAREA)