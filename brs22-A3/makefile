CC = gcc

CFLAGS = -g
	objects = A3.o enemy.o space.o

all: space_enemies

space_enemies: A3.o enemy.o space.o
	$(CC) -o space_enemies $(objects)

A3.o: A3.c
	$(CC) $(CFLAGS) -c A3.c

enemy.o: enemy.c
	$(CC) $(CFLAGS) -c enemy.c

space.o: space.c
	$(CC) $(CFLAGS) -c space.c

clean :
	rm -f $(objects) space_enemies		