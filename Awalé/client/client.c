#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client.h"

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
         //choix de quitter l'application
         end_connection(sock);
         break;
      case 1:
         //choix de lancer une game
         break;
      case 2:
         //choix d'afficher les joueurs
         break;
      case 3:
         //choix de communiquer
         printf("entrez 'quit' pour quitter\n");
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
               write_server(sock, buffer);
            }
            else if(FD_ISSET(sock, &rdfs))
            {
               int n = read_server(sock, buffer);
               /* server down */
               if(n == 0)
               {
                  printf("Server disconnected !\n");
                  break;
               }
               puts(buffer);
            }
         }
         break;
      default:
         break;
      }
   }   
}

static int init_connection(const char *address)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };
   struct hostent *hostinfo;

   if(sock == INVALID_SOCKET)
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
      printf("1. Option 1\n");
      printf("2. Option 2\n");
      printf("3. Option 3\n");
      printf("0. Quitter\n");
      printf("Veuillez choisir une option : ");   
      scanf("%d", &choix);
    return choix; // Retourne le choix valide
}