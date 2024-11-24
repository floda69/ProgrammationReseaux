#ifndef MESSAGE_H
#define MESSAGE_H

#include "jeu.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

// codes for our protocol
#define NAME_USED "00"
#define GLOBAL_MSG "01"
#define SWITCH_CHAT "02"
#define PLAYERS_LIST "03"
#define SERVER_MSG "04"
#define DEFY "05"
#define ACCEPT_INVITE "06"
#define DECLINE_INVITE "07"
#define GAME "08"
#define PLAY "09"
#define SPECTATE "10"
#define GAME_LIST "11"
#define PRIVATE_MSG "12"
#define RESIGN "13"
#define GET_BIO "14"
#define SET_BIO "15"

// input commands from user
#define CMD_DISCONNECT "/disconnect"
#define CMD_PLAYER_LIST "/players"
#define CMD_SWITCH_CHAT_MODE "/switch chat mode"
#define CMD_GLOBAL "/global"
#define CMD_DEFY_PLAYER "/defy "
#define CMD_ACCEPT_INVITE "/accept"
#define CMD_DECLINE_INVITE "/decline"
#define CMD_PLAY "/play"
#define CMD_SPECTATE "/spectate"
#define CMD_GAME_LIST "/games"
#define CMD_Private "/private"
#define CMD_RESIGN "/resign"
#define CMD_BIO "/bio"
#define CMD_BIO_SET "/setbio"

#define DISCONNECTED_SERVER "Serveur déconnecté, ressayer plus tard...\n"

// colors for format
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define COLOR_RESET "\033[1;0m"

const char* serialize_message(char *code, const char *message);

void serialize_awale(Awale *jeu, char *buffer, size_t buffer_size);
void deserialize_awale(Awale *jeu, const char *buffer);

#endif /* guard */