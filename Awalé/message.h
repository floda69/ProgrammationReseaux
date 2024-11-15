#ifndef MESSAGE_H
#define MESSAGE_H

#include "client/jeu.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

#define NAME "01"
#define CHAT_GENERAL "02"
#define PLAYERS_LIST "03"

const char* serialize_name(const char *name);

void serialize_coup(Awale *jeu, char *buffer);

void deserialize_coup(Awale *jeu, char *buffer);

#endif /* guard */