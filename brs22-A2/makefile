CFLAGS = -Wall -g  # Flags de compilação
LDFLAGS = 

CC = gcc
objects = main.o arff.o log.o

main: $(objects)
	$(CC) -o main $(objects)

arff.o: arff.c arff.h
	$(CC) $(CFLAGS) -c arff.c

log.o: log.c log.h
	$(CC) $(CFLAGS) -c log.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(objects) main
