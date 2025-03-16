# Définir le compilateur
CC = gcc

# Définir les options de compilation
CFLAGS = -Wall -I./include

# Définir les fichiers source
SRC = src/main.c src/image_rgb.c src/matrice.c src/YCbCr.c

# Définir les fichiers objets
OBJ = $(SRC:.c=.o)

# Définir l'exécutable
EXEC = my_program

# La règle par défaut pour construire l'exécutable
all: $(EXEC)

# Règle pour construire l'exécutable à partir des fichiers objets
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Règle pour générer les fichiers objets à partir des fichiers source
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers générés
clean:
	rm -f $(OBJ) $(EXEC)
