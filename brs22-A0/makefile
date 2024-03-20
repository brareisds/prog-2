CC = gcc

# arquivos-objeto
	objects = main.o libfila.o 
     
all: main.o libfila.o
	$(CC) -o main main.o libfila.o 

libfila.o: libfila.c
	$(CC) -c libfila.c

main.o: main.c
	$(CC) -c main.c

clean:
	rm -f $(objects) main
