#ifndef MESSAGE_H
#define MESSAGE_H

#include "client/jeu.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

// codes for our protocol
#define GLOBAL_MSG "01"
#define SWITCH_CHAT "02"
#define PLAYERS_LIST "03"
#define SERVER_MSG "04"
#define DEFY "05"
#define ACCEPT_INVITE "06"

// input commands from user
#define CMD_DISCONNECT "/disconnect"
#define CMD_PLAYER_LIST "/players"
#define CMD_SWITCH_CHAT_MODE "/switch chat mode"
#define CMD_GLOBAL "/global"
#define CMD_DEFY_PLAYER "/defy "

#define DISCONNECTED_SERVER "Serveur déconnecté, ressayer plus tard...\n"

// colors for format
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define COLOR_RESET "\033[1;0m"

const char* serialize_message(char *code, const char *message);

void serialize_coup(Awale *jeu, char *buffer);

void deserialize_coup(Awale *jeu, char *buffer);

#endif /* guard */