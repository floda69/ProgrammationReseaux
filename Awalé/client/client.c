#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client.h"
#include "../message.h"
#include "jeu.h"

static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

void discard_old_messages(int sock) {
    char temp_buffer[BUF_SIZE];
    int flags = MSG_DONTWAIT; // Non-blocking flag

    while (recv(sock, temp_buffer, BUF_SIZE - 1, flags) > 0) {
        // Discard the message
    }
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

   /* send our name */
   write_server(sock, name);
   int choice = -1 ;
   while (choice != 0)
   {
      choice = menu();
      switch (choice)
      {
      case 0:
         //choix quitter l'application
         end_connection(sock);
         break;
      case 1:
         //choix de lancer une partie
         char* opponent;
         strcpy(opponent,check_invite(sock, buffer, rdfs));
         int joueur = 0; // 0 pour le joueur 1, 1 pour le joueur 2
         if(opponent == 0)
         {
            printf(DISCONNECTED_SERVER);
            break;
         }
         if (opponent[0] == '\n')
         {
            send_invite(sock, buffer, rdfs);
            joueur = 1;
            discard_old_messages(sock);
            while (1){
               FD_ZERO(&rdfs);

               /* add STDIN_FILENO */
               FD_SET(STDIN_FILENO, &rdfs);

               /* add the socket */
               FD_SET(sock, &rdfs);

               if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
               {
                  perror("select()");
                  exit(errno);
               }
               if(FD_ISSET(sock, &rdfs))
               {
                  int n = read_server(sock, buffer);
                  /* server down */
                  if(n == 0)
                  {
                     printf(DISCONNECTED_SERVER);
                  }
                  break;
               }
            }
            if (buffer[0] == '\n') //no player
            {
               printf("joueur indisponible\n");
               break;
            }
            else opponent = buffer;
         }

         while(1){
            FD_ZERO(&rdfs);

            /* add STDIN_FILENO */
            FD_SET(STDIN_FILENO, &rdfs);

            /* add the socket */
            FD_SET(sock, &rdfs);
            if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
            {
               perror("select()");
               exit(errno);
            }
            // TODO réception serveur
            if(FD_ISSET(sock, &rdfs)){
               int n = read_server(sock, buffer);
               /* server down */
               if(n == 0)
               {
                  printf(DISCONNECTED_SERVER);
                  break;
               }
               puts(buffer);
               break;
            }
            // TODO réception keyboard
            if (FD_ISSET(STDIN_FILENO, &rdfs))
            {
               (fgets(buffer, BUF_SIZE - 1, stdin));
               if (!strcmp(buffer, "quit\n"))
               {
                  break;
               }
               else
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
               if (buffer[0] == '\0') // catch empty message
               {
                  continue;
               }
               write_server(sock, serialize_message(NEW_GAME, buffer));
            }

         }
         ////faire gestion du tour par tour
         
         printf("Lancement d'une nouvelle partie\n");
         Awale jeu;
         strncpy(jeu.j1, name, 50);
         strncpy(jeu.j2, buffer, 50);

         initialiser_jeu(&jeu);
         int case_choisie;

         printf("Bienvenue dans le jeu d'Awalé !\n");

         while (!fin_de_jeu(&jeu))
         {
            afficher_plateau(&jeu);
            printf("Joueur %d, choisissez une case (0 à 5) : ", joueur + 1);
            scanf("%d", &case_choisie);

            if (coup_valide(&jeu, joueur, case_choisie))
            {
                  jouer_coup(&jeu, joueur, case_choisie);
                  joueur = 1 - joueur; // Changement de joueur
            }
            else
            {
                  printf("Coup invalide, essayez encore.\n");
            }
         }
         printf("Fin du jeu !\n");
         printf("Score final - Joueur 1: %d, Joueur 2: %d\n", jeu.score[0], jeu.score[1]);
         game(name, buffer);
      
         break;
      case 2:
         //choix de communiquer
         printf("entrez 'quit' pour quitter\n");
         discard_old_messages(sock);
         while(1)
         {
            FD_ZERO(&rdfs);

            /* add STDIN_FILENO */
            FD_SET(STDIN_FILENO, &rdfs);

            /* add the socket */
            FD_SET(sock, &rdfs);

            if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
            {
               perror("select()");
               exit(errno);
            }

            /* something from standard input : i.e keyboard */
            if(FD_ISSET(STDIN_FILENO, &rdfs))
            {
               (fgets(buffer, BUF_SIZE - 1, stdin));
               if(!strcmp(buffer, "quit\n"))
               {
                  break;
               }
               else
               {
                  char *p = NULL;
                  p = strstr(buffer, "\n");
                  if(p != NULL)
                  {
                     *p = 0;
                  }
                  else
                  {
                     /* fclean */
                     buffer[BUF_SIZE - 1] = 0;
                  }
               }
               if (buffer[0] == 0) //catch empty message
               {
                  continue;
               }
               write_server(sock, serialize_message(CHAT_GENERAL, buffer));
            }
            else if(FD_ISSET(sock, &rdfs))
            {
               int n = read_server(sock, buffer);
               /* server down */
               if(n == 0)
               {
                  printf(DISCONNECTED_SERVER);
                  break;
               }
               puts(buffer);
            }
         }
         break;

      case 3:
            write_server(sock, PLAYERS_LIST);
            discard_old_messages(sock);
            while (1){
               FD_ZERO(&rdfs);

            /* add STDIN_FILENO */
            FD_SET(STDIN_FILENO, &rdfs);

            /* add the socket */
            FD_SET(sock, &rdfs);

            if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
            {
               perror("select()");
               exit(errno);
            }
            if(FD_ISSET(sock, &rdfs))
            {
               int n = read_server(sock, buffer);
               /* server down */
               if(n == 0)
               {
                  printf(DISCONNECTED_SERVER);
                  break;
               }
               puts(buffer);
               break;
            }
         }
         break;
      default:
         printf("Saisie non valide !\n");
         break;
      }
   }
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
      fprintf (stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

   sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
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

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      exit(errno);
   }

   buffer[n] = 0;

   return n;
}

static void write_server(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

static char* check_invite(SOCKET sock, char *buffer,fd_set rdfs)
{
   write_server(sock, serialize_message(ASK_INVITE, buffer));
   discard_old_messages(sock);
   while (1){
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the socket */
      FD_SET(sock, &rdfs);

      if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }
      if(FD_ISSET(sock, &rdfs))
      {
         int n = read_server(sock, buffer);
         return(buffer);
      }
   }
}

static void send_invite(SOCKET sock, char *buffer,fd_set rdfs)
{
   printf("entrez le nom du joueur à défier\n");
   while(1)
   {
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the socket */
      FD_SET(sock, &rdfs);

      if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      /* something from standard input : i.e keyboard */
      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
         (fgets(buffer, BUF_SIZE - 1, stdin));

         char *p = NULL;
         p = strstr(buffer, "\n");
         if(p != NULL)
         {
            *p = 0;
         }
         else
         {
            /* fclean */
            buffer[BUF_SIZE - 1] = 0;
         }
         if (buffer[0] == 0) //catch empty message
         {
            continue;
         }
         write_server(sock, serialize_message(NEW_GAME, buffer));
         break;
      }
   }
}

int main(int argc, char **argv)
{
   if(argc < 2)
   {
      printf("Usage : %s [address] [pseudo]\n", argv[0]);
      return EXIT_FAILURE;
   }

   init();

   app(argv[1], argv[2]);

   end();

   return EXIT_SUCCESS;
}

int menu() {
    int choix = -1;
      printf("\n=== Menu ===\n");
      printf("1. Lancer une partie\n");
      printf("2. Chat global\n");
      printf("3. Joueurs en ligne\n");
      printf("0. Quitter\n");
      printf("Veuillez choisir une option : ");   
      scanf("%d", &choix);
      // vérifier qu'on a bien un entier
      if (choix < 0 || choix > 3) {
         choix = 0;
      }
    return choix; // Retourne le choix valide
}