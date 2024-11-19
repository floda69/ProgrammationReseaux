#ifndef MESSAGE_H
#define MESSAGE_H

#include "client/jeu.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

#define GLOBAL_MSG "01"
#define SWITCH_CHAT "02"
#define PLAYERS_LIST "03"
#define NEW_GAME "04"
#define ASK_INVITE "05"

#define CMD_DISCONNECT "/disconnect"
#define CMD_PLAYER_LIST "/players"
#define CMD_SWITCH_CHAT_MODE "/switch chat mode"
#define CMD_GLOBAL "/global"

#define DISCONNECTED_SERVER "Serveur déconnecté, ressayer plus tard...\n"

const char* serialize_message(char *code, const char *message);

void serialize_coup(Awale *jeu, char *buffer);

void deserialize_coup(Awale *jeu, char *buffer);

#endif /* guard */