# Compiler et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -g

# Dossiers des sources
CLIENT_DIR = client
SERVEUR_DIR = serveur

# Fichiers sources
CLIENT_SOURCES = $(wildcard $(CLIENT_DIR)/*.c) message.c
SERVEUR_SOURCES = $(wildcard $(SERVEUR_DIR)/*.c) message.c

# Noms des exécutables
CLIENT_EXEC = $(CLIENT_DIR)/client_exec
SERVEUR_EXEC = $(SERVEUR_DIR)/serveur_exec

# Règle par défaut : compile les deux exécutables
all: $(CLIENT_EXEC) $(SERVEUR_EXEC)

# Compilation du client avec client.c et message.c
$(CLIENT_EXEC): $(CLIENT_SOURCES) message.h
	$(CC) $(CFLAGS) -o $@ $(CLIENT_SOURCES)

# Compilation du serveur avec serveur.c et message.c
$(SERVEUR_EXEC): $(SERVEUR_SOURCES) message
