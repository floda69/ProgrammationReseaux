/* Client pour les sockets
 *    socket_client ip_server port
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
//#include "jeu.h"

int main(int argc, char** argv )
{ 
  int    sockfd,newsockfd,clilen,chilpid,ok,nleft,nbwriten;
  char c;
  struct sockaddr_in cli_addr,serv_addr;

  if (argc!=3) {printf ("usage  socket_client server port\n");exit(0);}
 
 
  /*
   *  partie client 
   */
  printf ("client starting\n");  

  /* initialise la structure de donnee */
  bzero((char*) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family       = AF_INET;
  serv_addr.sin_addr.s_addr  = inet_addr(argv[1]);
  serv_addr.sin_port         = htons(atoi(argv[2]));
  
  /* ouvre le socket */
  if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {printf("socket error\n");exit(0);}
  
  /* effectue la connection */
  if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
    {printf("socket error\n");exit(0);}
    
    // demander nom
    // envoyer un nom
    // affichage menu

    // 1- liste joueurs 
    

  /* repete dans le socket tout ce qu'il entend */
  while (1)
  {
    c = getchar();
    write(sockfd, &c, 1);
    if (c == EOF) break;
  }
  close(sockfd);

}

// void deserialize_struct(char *buffer, Awale *jeu) {
//     size_t offset = 0;

//     // Copier plateau
//     memcpy(jeu->plateau, buffer + offset, sizeof(jeu->plateau));
//     offset += sizeof(jeu->plateau);
//     offset++; // Skip the newline character

//     // Copier score
//     memcpy(jeu->score, buffer + offset, sizeof(jeu->score));
//     offset += sizeof(jeu->score);
//     offset++; // Skip the newline character

//     // Copier j1
//     memcpy(jeu->j1, buffer + offset, sizeof(jeu->j1));
//     offset += sizeof(jeu->j1);
//     offset++; // Skip the newline character

//     // Copier j2
//     memcpy(jeu->j2, buffer + offset, sizeof(jeu->j2));
//     offset += sizeof(jeu->j2);
//     offset++; // Skip the newline character
// }