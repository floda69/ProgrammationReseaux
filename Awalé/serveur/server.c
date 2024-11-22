#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server.h"
#include "client.h"

static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if (err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}

static void app(void)
{
   SOCKET sock = init_connection();
   char buffer[BUF_SIZE];
   /* the index for the array */
   int actual = 0;
   int max = sock;
   /* an array for all clients */
   Client clients[MAX_CLIENTS];
   Awale games[MAX_CLIENTS];
   int gameIndex = 0;

   fd_set rdfs;

   while (1)
   {
      int i = 0;
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the connection socket */
      FD_SET(sock, &rdfs);

      /* add socket of each client */
      for (i = 0; i < actual; i++)
      {
         FD_SET(clients[i].sock, &rdfs);
      }

      if (select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      /* something from standard input : i.e keyboard */
      if (FD_ISSET(STDIN_FILENO, &rdfs))
      {
         /* stop process when type on keyboard */
         break;
      }
      else if (FD_ISSET(sock, &rdfs))
      {
         /* new client */
         SOCKADDR_IN csin = { 0 };
         size_t sinsize = sizeof csin;
         int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
         if(csock == SOCKET_ERROR)
         {
            perror("accept()");
            continue;
         }

         /* after connecting the client sends its name */
         if (read_client(csock, buffer) == -1)
         {
            /* disconnected */
            continue;
         }

         /* what is the new maximum fd ? */
         max = csock > max ? csock : max;
         
         int name_used = 0;
         for (int i = 0; i < actual; i++){
            if (strcmp(clients[i].name, buffer) == 0){
               write_client(csock, NAME_USED);
               name_used = 1;
               break;
            }
         }
         if (name_used){
            continue;
         }

         FD_SET(csock, &rdfs);

         Client c = {.sock = csock, .isInGame = 0, .isInGlobalChatMode = 1, .invite = 0};
         strncpy(c.name, buffer, NAME_SIZE - 1);
         clients[actual] = c;
         actual++;

         strncat(buffer, " just connected !\n", BUF_SIZE - strlen(buffer) - 1);
         send_message_to_all_clients(clients, c, actual, buffer, 1);
      }
      else
      {
         int i = 0;
         for (i = 0; i < actual; i++)
         {
            /* a client is talking */
            if (FD_ISSET(clients[i].sock, &rdfs))
            {
               Client client = clients[i];
               int c = read_client(clients[i].sock, buffer);
               /* client disconnected */
               if (c == 0)
               {
                  closesocket(clients[i].sock);
                  remove_client(clients, i, &actual);
                  strncpy(buffer, client.name, BUF_SIZE - 1);
                  strncat(buffer, " disconnected !\n", BUF_SIZE - strlen(buffer) - 1);
                  send_message_to_all_clients(clients, client, actual, buffer, 1);
               }
               else
               {
                  // décode message
                  if (strncmp(buffer, PLAYERS_LIST, 2) == 0)
                  {
                     send_clients_list_on_demand(clients, client, actual);
                  }
                  else if (strncmp(buffer, SWITCH_CHAT, 2) == 0)
                  {
                     switch_client_chat_mode(&(clients[i]));
                  }
                  else if (strncmp(buffer, GLOBAL_MSG, 2) == 0)
                  {
                     send_message_to_all_clients(clients, client, actual, buffer + 3, 0);
                  }
                  else if (strncmp(buffer, DEFY, 2) == 0)
                  {
                     defy_player(clients, client, actual, buffer + 2);
                  }
                  else if (strncmp(buffer, ACCEPT_INVITE,2)==0){
                     accept_invite(clients, client, actual);
                     launch_game(clients, client, actual, games, &gameIndex);
                  }
                  else if (strncmp(buffer, DECLINE_INVITE,2)==0){
                     decline_invite(clients, client, actual);
                  }
               }
               break;
            }
         }
      }
   }

   clear_clients(clients, actual);
   end_connection(sock);
}

static void clear_clients(Client *clients, int actual)
{
   int i = 0;
   for (i = 0; i < actual; i++)
   {
      closesocket(clients[i].sock);
   }
}

static void remove_client(Client *clients, int to_remove, int *actual)
{
   /* we remove the client in the array */
   memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
   /* number client - 1 */
   (*actual)--;
}

static void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server)
{
   /* we don't send message if not in global chat mode*/
   if (sender.isInGlobalChatMode)
   {
      int i = 0;
      char message[BUF_SIZE];
      message[0] = 0;
      for (i = 0; i < actual; i++)
      {
         if ((sender.sock != clients[i].sock) && (clients[i].isInGlobalChatMode == 1))
         {
            strncpy(message, GLOBAL_MSG, BUF_SIZE - 1);
            strncat(message, GREEN, sizeof message - strlen(message) - 1);
            if (!from_server)
            {
               strncat(message, YELLOW, sizeof message - strlen(message) - 1);
               strncat(message, sender.name, sizeof message - strlen(message) - 1);
               strncat(message, " : ", sizeof message - strlen(message) - 1);
            }
            strncat(message, buffer, sizeof message - strlen(message) - 1);
            strncat(message, COLOR_RESET, sizeof message - strlen(message) - 1);
            write_client(clients[i].sock, message);
         }
      }
   }
}

static void send_message_to_client(Client receiver, const char *buffer)
{
   char message[BUF_SIZE];
   message[0] = 0;
   strncpy(message, SERVER_MSG, BUF_SIZE - 1);
   strncat(message, BLUE, sizeof message - strlen(message) - 1);
   strncat(message, buffer, sizeof message - strlen(message) - 1);
   strncat(message, COLOR_RESET, sizeof message - strlen(message) - 1);
   write_client(receiver.sock, message);
}

static void defy_player(Client *clients, Client defier, int actual, const char *playerDefied)
{
   int i = 0;
   char message[BUF_SIZE];
   for (i = 0; i < actual; i++)
   {
      if (!strcmp(playerDefied, clients[i].name) && clients[i].invite[0] == 0 && defier.invite[0] == 0 && strcmp(defier.name, playerDefied))
      {
         strcpy(clients[i].invite, defier.name);
         int index = get_index_by_name(clients, defier.name, actual);
         clients[index].invite[0] = 1;
         strcpy(&clients[index].invite[1], playerDefied);
         strncpy(message, "Vous avez été défié par ", BUF_SIZE - 1);
         strcat(message, GREEN);
         strcat(message, defier.name);
         strcat(message, BLUE);
         strcat(message, "\nVoulez-vous accepter ?\n");
         send_message_to_client(clients[i], message);
         send_message_to_client(defier, "Demande envoyée");
         return;
      }
   }
   send_message_to_client(defier, "impossible d'envoyer la demande");
}

static void decline_invite(Client *clients, Client client, int actual){
   int i = 0;
   if (client.invite[0] == 0)
   {
      send_message_to_client(client, "Pas de demande en cours");
      return;
   }
   if (client.invite[0] == 1)
   {
      send_message_to_client(client, "Annulation de la demande");
      int index = get_index_by_name(clients, &client.invite[1], actual);
      clients[index].invite[0] = 0;
      client.invite[0] = 0;
      send_message_to_client(clients[index], "Demande annulée");
      return;
   }
   if (client.invite[0] == 2)
   {
      send_message_to_client(client, "Impossible pendant une partie");
      return;
   }
   for (i = 0; i < actual; i++)
   {
      if (!strcmp(client.invite, clients[i].name))
      {
         send_message_to_client(clients[i], "Demande refusée");
         int index = get_index_by_name(clients, client.name, actual);
         clients[index].invite[0] = 0;
         clients[i].invite[0] = 0;
         return;
      }
   }
}

static void accept_invite(Client *clients, Client client, int actual){
   int i = 0;
   if (client.invite[0] == 1)
   {
      send_message_to_client(client, "Attendez la réponse du joueur défié");
      return;
   }
   if (client.invite[0] == 2)
   {
      send_message_to_client(client, "Impossible pendant une partie");
      return;
   }
   for (i = 0; i < actual; i++)
   {
      if (!strcmp(client.invite, clients[i].name))
      {
         int index = get_index_by_name(clients, client.name, actual);
         clients[index].isInGame = 1;
         clients[i].isInGame = 1;
         clients[index].invite[0] = 2;
         clients[i].invite[0] = 2;
         break;
      }
   }
}

static void launch_game(Client *clients, Client client, int actual, Awale *games, int *gameIndex)
{
   Awale jeu;
   jeu.turn = rand() % 2;
   Client opponent = clients[get_index_by_name(clients, client.invite, actual)];
   strncpy(jeu.j1, client.name, 50);
   strncpy(jeu.j2, opponent.name , 50);
   initialiser_jeu(&jeu);
   games[*gameIndex] = jeu;
   (*gameIndex)++;
   send_message_to_client(client, "Bienvenue dans le jeu d'Awalé !\n");
   send_message_to_client(opponent, "Demande acceptée\nBienvenue dans le jeu d'Awalé !\n");
   usleep(100000); //wait to be sure the messages are separated
   send_game(&jeu, clients, actual);
}

static void send_game(Awale *game, Client* clients, int actual)
{
   char message[BUF_SIZE];
   memset(message, 0, BUF_SIZE);
   strncpy(message, GAME, BUF_SIZE - 1);
   serialize_awale(game, message + 2, BUF_SIZE - 3);
   int index1 = get_index_by_name(clients, game->j1, actual);
   int index2 = get_index_by_name(clients, game->j2, actual);
   write_client(clients[index1].sock, message);
   write_client(clients[index2].sock, message);
}

static void send_clients_list_on_demand(Client *clients, Client client, int actual)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   int clients_online = 0;

   strncpy(message, PLAYERS_LIST, BUF_SIZE - 1);
   strncat(message, BLUE, sizeof message - strlen(message) - 1);
   strncat(message, "Liste des joueurs: \n", sizeof message - strlen(message) - 1);

   for (i = 0; i < actual; i++)
   {
      if (client.sock != clients[i].sock)
      {
         strncat(message, clients[i].name, sizeof message - strlen(message) - 1);
         strncat(message, "\n", sizeof message - strlen(message) - 1);
         clients_online++;
      }
   }

   if (clients_online == 0)
   {
      strncpy(message, "Aucun autre joueur en ligne.\n", sizeof message - strlen(message) - 1);
   }

   strncat(message, COLOR_RESET, sizeof message - strlen(message) - 1);

   write_client(client.sock, message);
}

static void switch_client_chat_mode(Client *client)
{

   // puts(client->name);
   printf("%s passe du mode chat global %d", client->name, client->isInGlobalChatMode);
   int tempSwitch = client->isInGlobalChatMode;
   client->isInGlobalChatMode = (tempSwitch == 0) ? 1 : 0;
   printf(" à %d\n", client->isInGlobalChatMode);
}

static int init_connection(void)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = {0};

   if (sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if (bind(sock, (SOCKADDR *)&sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   if (listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }

   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static int read_client(SOCKET sock, char *buffer)
{
   int n = 0;

   if ((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      /* if recv error we disonnect the client */
      n = 0;
   }

   buffer[n] = 0;

   return n;
}

static void write_client(SOCKET sock, const char *buffer)
{
   if (send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

static int get_index_by_name(Client *clients, char *name, int actual)
{
   int i = 0;
   for (i = 0; i < actual; i++)
   {
      if (!strcmp(name, clients[i].name))
      {
         return i;
      }
   }
}


// static void search_opponent(Client *clients, Client client, int actual, const char *buffer)
// {
//    int i = 0;
//    char message[BUF_SIZE];
//    message[0] = '\n';
//    for (i = 0; i < actual; i++)
//    {
//       if (!strcmp(buffer, clients[i].name) && clients[i].isInGame == 0)
//       {
//          strncpy(message, clients[i].name, BUF_SIZE - 1);
//          strncpy(clients[i].invite, client.name, 50);
//          clients[i].isInGame = 1;
//          client.isInGame = 1;
//          break;
//       }
//    }
//    write_client(client.sock, message);
// }

// static void check_invite(Client client)
// {
//    char message[BUF_SIZE];
//    message[0] = '\n';
//    if (client.invite[0] != '\0')
//    {
//       strncpy(message, client.invite, BUF_SIZE - 1);
//       client.invite[0] = '\0';
//    }
//    write_client(client.sock, message);
// }

int main(int argc, char **argv)
{
   init();

   app();

   end();

   return EXIT_SUCCESS;
}
