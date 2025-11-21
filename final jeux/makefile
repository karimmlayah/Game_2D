CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LIBS = -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx -lSDL_mixer -lm
OBJ = main.o background.o enemies.o fonction.o enigme.o tik-tak.o menu.o
EXEC = prog

# Default target
all: $(EXEC)

# Link the program
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Compile main.o
main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile perso.o
perso.o: perso.c perso.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile menu.o
menu.o: menu.c menu.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile tik-tak.o
tik-tak.o: tik-tak.c tik-tak.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile background.o
background.o: background.c background.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile enemies.o
enemies.o: enemies.c enemies.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile fonction.o
fonction.o: fonctions.c fonction.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile enigme.o
enigme.o: enigme.c enigme.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the build
clean:
	rm -f $(OBJ) $(EXEC)

