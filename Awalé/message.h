#ifndef MESSAGE_H
#define MESSAGE_H

#include "server.h"

#define PARTIE 1

void serialize_coup(Awale *jeu, char *buffer);

void deserialize_coup(Awale *jeu, char *buffer);

#endif /* guard */