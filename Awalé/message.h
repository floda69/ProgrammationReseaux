#ifndef MESSAGE_H
#define MESSAGE_H

#include "client/jeu.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

#define NAME "01"
#define CHAT_GENERAL "02"
#define PLAYERS_LIST "03"
#define CHAT_PRIVE "04"

const char* serialize_message(char *code, const char *message);

void serialize_coup(Awale *jeu, char *buffer);

void deserialize_coup(Awale *jeu, char *buffer);

#endif /* guard */