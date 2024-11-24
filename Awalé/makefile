# Compiler et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -g

# Dossiers des sources
CLIENT_DIR = client
SERVEUR_DIR = serveur

# Fichiers sources
CLIENT_SOURCES = $(wildcard $(CLIENT_DIR)/*.c) message.c jeu.c
SERVEUR_SOURCES = $(wildcard $(SERVEUR_DIR)/*.c) message.c jeu.c cJSON.c persistance.c

# Noms des exécutables
CLIENT_EXEC = $(CLIENT_DIR)/client_exec
SERVEUR_EXEC = $(SERVEUR_DIR)/serveur_exec

# Règle par défaut : compile les deux exécutables
all: $(CLIENT_EXEC) $(SERVEUR_EXEC)

# Compilation du client avec client.c, jeu.c et message.c
$(CLIENT_EXEC): $(CLIENT_SOURCES)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_SOURCES)

# Compilation du serveur avec serveur.c et message.c
$(SERVEUR_EXEC): $(SERVEUR_SOURCES)
	$(CC) $(CFLAGS) -o $@ $(SERVEUR_SOURCES)

# Nettoyage des fichiers objets et exécutables
clean:
	rm -f $(CLIENT_EXEC) $(SERVEUR_EXEC)

# Règle pour relancer la compilation après un nettoyage
rebuild: clean all

# Indique les règles qui ne correspondent pas à des fichiers
.PHONY: all clean rebuild
