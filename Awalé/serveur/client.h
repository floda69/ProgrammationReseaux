#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"
#include "../message.h"

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
   char bio[200];
}Client;

#endif /* guard */
