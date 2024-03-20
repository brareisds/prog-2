# Makefile for compiling invaders.c and ship.c with Allegro 5

# Compiler
CC = gcc

# Source files
SOURCES = invaders.c ship.c enemy.c Joystick.c pistol.c bullet.c obstacle.c

# Output executable
TARGET = invaders

# Compiler flags and libraries
CFLAGS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 --cflags)
LIBS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 --libs) -lm

# Build the executable
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

# Clean the compiled files
clean:
	rm -f $(TARGET) last_score.bin
