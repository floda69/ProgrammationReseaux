#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"
#include "../message.h"

typedef struct
{
   SOCKET sock;
   char name[NAME_SIZE];
   int isInGame;
   int isInGlobalChatMode;
   char invite[NAME_SIZE+1];
   char bio[BIO_SIZE];
}Client;

#endif /* guard */
