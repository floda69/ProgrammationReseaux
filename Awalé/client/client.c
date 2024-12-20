#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client.h"
#include "../message.h"
#include "../jeu.h"

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

static void app(const char *address, const char *name)
{
   SOCKET sock = init_connection(address);
   char buffer[BUF_SIZE];

   fd_set rdfs;
   Awale jeu;
   int connected = 1;

   /* send our name */
   write_server(sock, name);
   while (connected)
   {
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the socket */
      FD_SET(sock, &rdfs);

      if (select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      /* something from standard input : i.e keyboard */
      if (FD_ISSET(STDIN_FILENO, &rdfs))
      {
         fgets(buffer, BUF_SIZE - 1, stdin);
         {
            char *p = NULL;
            p = strstr(buffer, "\n");
            if (p != NULL)
            {
               *p = 0;
            }
            else
            {
               /* fclean */
               buffer[BUF_SIZE - 1] = 0;
            }
         }
         // déconnection
         if (!strcmp(buffer, CMD_DISCONNECT))
            connected = 0;
         // afficher liste des joueurs connectés
         else if (!strcmp(buffer, CMD_PLAYER_LIST))
            write_server(sock, PLAYERS_LIST);
         // passer en mode chat global
         else if (!strcmp(buffer, CMD_SWITCH_CHAT_MODE))
            write_server(sock, SWITCH_CHAT);
         // envoyer message global
         else if (!strncmp(buffer, CMD_GLOBAL, strlen(CMD_GLOBAL)))
         {
            memmove(buffer, buffer + strlen(CMD_GLOBAL), strlen(buffer) - strlen(CMD_GLOBAL) + 1);
            write_server(sock, serialize_message(GLOBAL_MSG, buffer));
         }
         // défier un joueur
         else if (!strncmp(buffer, CMD_DEFY_PLAYER, strlen(CMD_DEFY_PLAYER)))
         {
            memmove(buffer, buffer + strlen(CMD_DEFY_PLAYER), strlen(buffer) - strlen(CMD_DEFY_PLAYER) + 1);
            write_server(sock, serialize_message(DEFY, buffer));
         }
         // accepter un défi
         else if (!strcmp(buffer, CMD_ACCEPT_INVITE)){
            memmove(buffer, buffer + strlen(CMD_ACCEPT_INVITE), strlen(buffer) - strlen(CMD_ACCEPT_INVITE) + 1);
            write_server(sock, serialize_message(ACCEPT_INVITE, ""));
         }
         // refuser un défi
         else if (!strcmp(buffer, CMD_DECLINE_INVITE)){
            memmove(buffer, buffer + strlen(CMD_DECLINE_INVITE), strlen(buffer) - strlen(CMD_DECLINE_INVITE) + 1);
            write_server(sock, serialize_message(DECLINE_INVITE, ""));
         }
         // jouer un coup
         else if (!strncmp(buffer, CMD_PLAY, strlen(CMD_PLAY))){
            // la validité d'un coup est verifiée avant envoi de la requête
            memmove(buffer, buffer + strlen(CMD_PLAY) + 1, strlen(buffer) - strlen(CMD_PLAY));
            if (coup_valide(&jeu, jeu.turn, *buffer - '0'))
            {
                  write_server(sock, serialize_message(PLAY, buffer));
            }
            else
            {
               printf("%sCoup invalide, essayez encore.%s\n",RED, COLOR_RESET);
            }
         }
         // regarder la partie d'un joueur
         else if (!strncmp(buffer, CMD_SPECTATE, strlen(CMD_SPECTATE))){
            memmove(buffer, buffer + strlen(CMD_SPECTATE) + 1, strlen(buffer) - strlen(CMD_SPECTATE));
            write_server(sock, serialize_message(SPECTATE, buffer));
         }
         // lister les parties en cours
         else if (!strncmp(buffer, CMD_GAME_LIST, strlen(CMD_GAME_LIST))){
            write_server(sock, serialize_message(GAME_LIST, ""));
         }
         // envoyer un message privé
         else if (!strncmp(buffer, CMD_PRIVATE, strlen(CMD_PRIVATE))){
            memmove(buffer, buffer + strlen(CMD_PRIVATE) + 1, strlen(buffer) - strlen(CMD_PRIVATE));
            write_server(sock, serialize_message(PRIVATE_MSG, buffer));
         }
         // abandonner la partie
         else if (!strncmp(buffer, CMD_RESIGN, strlen(CMD_RESIGN))){
            write_server(sock, serialize_message(RESIGN, ""));
         }
         else if (!strncmp(buffer, CMD_BIO, strlen(CMD_BIO))){
            memmove(buffer, buffer + strlen(CMD_BIO) + 1, strlen(buffer) - strlen(CMD_BIO));
            write_server(sock, serialize_message(GET_BIO, buffer));
         }
         else if (!strncmp(buffer, CMD_BIO_SET, strlen(CMD_BIO_SET))){
            memmove(buffer, buffer + strlen(CMD_BIO_SET) + 1, strlen(buffer) - strlen(CMD_BIO_SET));
            write_server(sock, serialize_message(SET_BIO, buffer));
         }
         // ajouter un ami
         else if (!strncmp(buffer, CMD_ADD_FRIEND, strlen(CMD_ADD_FRIEND))){
            memmove(buffer, buffer + strlen(CMD_ADD_FRIEND) + 1, strlen(buffer) - strlen(CMD_ADD_FRIEND));
            write_server(sock, serialize_message(ADD_FRIEND, buffer));
         }
         // changer la restriction spectateur
         else if (!strncmp(buffer, CMD_SWITCH_PRIVACY, strlen(CMD_SWITCH_PRIVACY))){
            memmove(buffer, buffer + strlen(CMD_SWITCH_PRIVACY) + 1, strlen(buffer) - strlen(CMD_SWITCH_PRIVACY));
            write_server(sock, serialize_message(SWITCH_PRIVACY, buffer));
         }
         // changer le chemin du fichier de sauvegarde
         else if (!strncmp(buffer, CMD_SAVE_FILE, strlen(CMD_SAVE_FILE))){
            memmove(buffer, buffer + strlen(CMD_SAVE_FILE) + 1, strlen(buffer) - strlen(CMD_SAVE_FILE));
            write_server(sock, serialize_message(ADD_SAVE_FILE, buffer));
         }
         // afficher une partie sauvée
         else if (!strncmp(buffer, CMD_LOAD_SAVE, strlen(CMD_LOAD_SAVE))){
            memmove(buffer, buffer + strlen(CMD_LOAD_SAVE) + 1, strlen(buffer) - strlen(CMD_LOAD_SAVE));
            write_server(sock, serialize_message(LOAD_SAVE_FILE, buffer));
         }
         else
            printf("%sInvalid command.%s\n", RED, COLOR_RESET);
      }
      else if (FD_ISSET(sock, &rdfs)) // message from server
      {
         int n = read_server(sock, buffer);
         /* server down */
         if (n == 0)
         {
            puts("Server disconnected !");
            break;
         }
         else if (!strncmp(buffer, GAME, 2))
         {
            deserialize_awale(&jeu, buffer + 2);
            afficher_plateau(&jeu);
            if ((strcmp(jeu.j1, name)==0 && jeu.turn==0) || (strcmp(jeu.j2, name)==0 && jeu.turn==1)) 
               printf("%s%s, choisissez une case (0 à 5) %s\n", MAGENTA, name, COLOR_RESET);
         }
         else if (!strncmp(buffer, NAME_USED,2))
         {
            printf("Nom déjà utilisé, veuillez en choisir un autre\n");
            exit(0);
         }
         else if (!strncmp(buffer, SERVER_MSG, 2))
         {
            puts(buffer + 2);
         }
         else if (!strncmp(buffer, PLAYERS_LIST, 2))
         {
            puts(buffer + 2);
         }
         else if (!strncmp(buffer, GLOBAL_MSG, 2))
         {
            puts(buffer + 2);
         }
         else if (!strncmp(buffer, PRIVATE_MSG, 2))
         {
            puts(buffer + 2);
         }
         else if (!strncmp(buffer, GAME_LIST, 2))
         {
            puts(buffer + 2);
         }
      }
   }
   end_connection(sock);
}

static int init_connection(const char *address)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = {0};
   struct hostent *hostinfo;

   if (sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   hostinfo = gethostbyname(address);
   if (hostinfo == NULL)
   {
      fprintf(stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

   sin.sin_addr = *(IN_ADDR *)hostinfo->h_addr;
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if (connect(sock, (SOCKADDR *)&sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
   {
      perror("connect()");
      exit(errno);
   }

   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static int read_server(SOCKET sock, char *buffer)
{
   int n = 0;

   if ((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      exit(errno);
   }

   buffer[n] = 0;

   return n;
}

static void write_server(SOCKET sock, const char *buffer)
{
   if (send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

int main(int argc, char **argv)
{
   if (argc < 2)
   {
      printf("Usage : %s [address] [pseudo]\n", argv[0]);
      return EXIT_FAILURE;
   }

   init();

   app(argv[1], argv[2]);

   end();

   return EXIT_SUCCESS;
}
