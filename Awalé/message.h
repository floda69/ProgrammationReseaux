#ifndef MESSAGE_H
#define MESSAGE_H

#include "client/jeu.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

#define NAME "01"
#define CHAT_GENERAL "02"
#define PLAYERS_LIST "03"
#define NEW_GAME "04"
#define ASK_INVITE "05"
#define INGAME "05"


#define DISCONNECTED_SERVER "Serveur déconnecté, ressayer plus tard...\n"

const char* serialize_message(char *code, const char *message);

void serialize_coup(Awale *jeu, char *buffer);

void deserialize_coup(Awale *jeu, char *buffer);

#endif /* guard */