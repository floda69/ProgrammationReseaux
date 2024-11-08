/* Serveur sockets TCP
 * affichage de ce qui arrive sur la socket
 *    socket_server port (port > 1024 sauf root)
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "jeu.h"

int main(int argc, char** argv )
{ char datas[] = "hello\n";
  int    sockfd,newsockfd,clilen,chilpid,ok,nleft,nbwriten;
  char c;
  struct sockaddr_in cli_addr,serv_addr;
  

  if (argc!=2) {printf ("usage: socket_server port\n");exit(0);}
 
  printf ("server starting...\n");  
  
  /* ouverture du socket */
  sockfd = socket (AF_INET,SOCK_STREAM,0);
  if (sockfd<0) {printf ("impossible d'ouvrir le socket\n");exit(0);}

  /* initialisation des parametres */
  bzero((char*) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family       = AF_INET;
  serv_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
  serv_addr.sin_port         = htons(atoi(argv[1]));

  /* effecture le bind */
  if (bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
     {printf ("impossible de faire le bind\n");exit(0);}

  /* petit initialisation */
  listen(sockfd,1);
     
  /* attend la connection d'un client */
  clilen = sizeof (cli_addr);
  
  while (1) {
    newsockfd = accept (sockfd,(struct sockaddr*) &cli_addr, &clilen);
    if (newsockfd<0) {printf ("accept error\n"); exit(0);}
    printf ("connection accepted\n");
    int pid = fork();
    if (pid == 0) {
    break;
    }
  }

  // protocole de communication: 
  // 0 : 0 ou 1
  
  // if 0, décoder : char* j1 name, char* j2 name, int j1 score, int j2 score, int** plateau [][]
  // if 1, décoder : char* message
  

  // Initialisation du jeu

  // boucle tant que non fin de jeu:

  // dire au client de jouer

  // attendre de recevoir chiffre du client
  
  // faire le coup

  // envoyer jeu aux clients

  while (1){ 
    while (read(newsockfd,&c,1)!=1);
    if (c == EOF) break;
    printf("%c",c);
  }
  printf("disconnected\n");
  shutdown (sockfd, 2);  
   /*  attention il s'agit d'une boucle infinie 
    *  le socket nn'est jamais ferme !
    */

   return 1;
 }

void serialize_struct(Awale *jeu, char *buffer) {
    size_t offset = 0;

    // Copier plateau
    memcpy(buffer + offset, jeu->plateau, sizeof(jeu->plateau));
    offset += sizeof(jeu->plateau);
    buffer[offset++] = '\n';

    // Copier score
    memcpy(buffer + offset, jeu->score, sizeof(jeu->score));
    offset += sizeof(jeu->score);
    buffer[offset++] = '\n';

    // Copier j1
    memcpy(buffer + offset, jeu->j1, sizeof(jeu->j1));
    offset += sizeof(jeu->j1);
    buffer[offset++] = '\n';

    // Copier j2
    memcpy(buffer + offset, jeu->j2, sizeof(jeu->j2));
    offset += sizeof(jeu->j2);
    buffer[offset++] = '\n';
}