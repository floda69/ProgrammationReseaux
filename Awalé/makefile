# Compiler et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -g

# Dossiers des sources
CLIENT_DIR = client
SERVEUR_DIR = serveur

# Recherche automatique de tous les fichiers .c dans chaque dossier
CLIENT_SOURCES = $(wildcard $(CLIENT_DIR)/*.c)
SERVEUR_SOURCES = $(wildcard $(SERVEUR_DIR)/*.c)

# Noms des exécutables
CLIENT_EXEC = $(CLIENT_DIR)/client_exec
SERVEUR_EXEC = $(SERVEUR_DIR)/serveur_exec

# Règle par défaut : compile les deux exécutables
all: $(CLIENT_EXEC) $(SERVEUR_EXEC)

# Compilation du client avec tous les fichiers sources dans le dossier client
$(CLIENT_EXEC): $(CLIENT_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation du serveur avec tous les fichiers sources dans le dossier serveur
$(SERVEUR_EXEC): $(SERVEUR_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

# Nettoyage des fichiers objets et exécutables
clean:
	rm -f $(CLIENT_EXEC) $(SERVEUR_EXEC)

# Règle pour relancer la compilation après un nettoyage
rebuild: clean all

# Indique les règles qui ne correspondent pas à des fichiers
.PHONY: all clean rebuild

