# Nome do executável
TARGET = AS

# Compilador
CC = gcc

# Opções do compilador
CFLAGS = -Wall -g $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --cflags)

# Bibliotecas necessárias
LIBS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs)

# Arquivos fonte
SRCS = AggressiveSquares.c Square.c Joystick.c Bullet.c Pistol.c

# Arquivos objeto
OBJS = $(SRCS:.c=.o)

# Regras
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
