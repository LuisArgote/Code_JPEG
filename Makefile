# Compilateur
CC = gcc

# Flags de compilation
CFLAGS = -Wall -Wextra $(shell pkg-config --cflags gtk4 cairo)

# Librairies à lier - L'ORDRE EST CRUCIAL
LDLIBS = $(shell pkg-config --libs gtk4 cairo) -lm

# Ajoutez cette ligne dans les flags de compilation
CFLAGS += -Iinclude -Isrc  # Cherche les headers dans include/ et src/

# Nom de l'exécutable
TARGET = my_program

# Fichiers sources
SRCS = src/main.c src/imageRGB.c src/matrice.c src/YCbCr.c src/graphique.c src/AC_DC.c src/RRSS.c src/huffman.c src/jpeg.c

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Règle par défaut
all: $(TARGET)

# Construction de l'exécutable - Notez l'ordre des librairies
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS)

# Génération des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean