# Compilateur et options
CC = gcc
CFLAGS = -Wall -g

# Fichiers sources
SRC = client.c

# Nom de l'exécutable
EXEC = client

# Bibliothèque fournie
LIBS = -L. -lseg

# Règle principale : construire l'exécutable
all: $(EXEC)

# Comment construire l'exécutable à partir du fichier source
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) $(LIBS)

# Nettoyage des fichiers générés
clean:
	rm -f $(EXEC) *.o
