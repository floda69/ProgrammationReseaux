/* 
Comer, D. E. and D. L. Stevens [1996], "Internetworking with TCP/IP Vol 3:
	Client-Server Programming and Applications," Prentice-Hall,
	Upper Saddle River, New Jersey.

Voici une version en francais du chapitre 22 ET qui peut etre compilee sous 
Linux : 

Reading specs from /usr/local/lib/gcc-lib/i686-pc-linux-gnu/2.95.2/specs
gcc version 2.95.2 19991024 (release)

Pour compiler, simplement taper make

*/

/* rdict.c - main, CommandeSuivante */
/* Le client interroge un dictionnaire (implant� sous forme de tableau)
   r�sidant sur la machine serveur.

   Les commandes possibles sont: I pour initialiser � z�ro le dictionnaire
                                 i pour ins�rer un mot
                                 s pour supprimer un mot
                                 c pour chercher si un mot existe
                                 q pour quitter

   Exemple de session:

   Vu du serveur:
   [root@dusty RPCGEN2]# ./rdictd
   Initialisation du dictionnaire ...
   
   
   Vu du client:
   [root@dusty RPCGEN2]# ./rdict dusty
   Commande: I
   Dictionnaire initialis� � vide.
   Commande: i
   Mot: Robert
   Robert inser�.
   Commande: i
   Mot: Jules
   Jules inser�.
   Commande: l
   Mot: toto
   commande l invalide (entrez I,s,c,q).
   Commande: c
   Mot: toto
   toto non trouv�.
   Commande: c
   Mot: Jules
   Jules trouv�.
   Commande: s
   Mot: Jules
   Jules supprim�.
   Commande: c
   Mot: Jules
   Jules non trouv�.
   Commande: c
   Mot: Robert
   Robert trouv�.
   Commande: q
   fin de session.
   [root@dusty RPCGEN2]# 

*/

#include <rpc/rpc.h>

#include <stdio.h>
#include <ctype.h>

#include "rdict.h"

#define	TAILLEMOT 50		/* Taille maximale d'un mot du dictionnaire */

#define	RSERVEUR "localhost"	/* nom du "remote host" serveur */
                                /* � passer en param�tre argv[1] */
CLIENT	*handle;		/* handle de la proc�dure serveur */

/*------------------------------------------------------------------------
 * main - insere, supprime, ou cherche un mot dans le dictionnaire
 *------------------------------------------------------------------------
 */
int main(int argc, char* argv[])
{
	char	Mot[TAILLEMOT+1];  /* allocation de la m�moire pour un mot */
	char	Commande;          /* commande saisie */
	int	LongueurMot;	   /* longueur r�elle du mot saisi */

      /* connexion avec le serveur pour les appels RPC  */
	
       handle = clnt_create(argv[1], RDICTPROG, RDICTVERS, "tcp");
       if (handle == 0) {
               printf("La connexion avec le programme distant ne peut etre �tablie...\n");
               exit(1);
       }

       while (1) {
	        printf("\n\n(I)nitialiser,(i)ns�rer,(s)upprimer,(c)chercher,(q)uitter\n");
	        LongueurMot = CommandeSuivante(&Commande, Mot);

		switch (Commande) {
		case 'I':	/* "initialise" */
			initialise();
			printf("Dictionnaire initialis� � vide.\n");
			break;

		case 'i':	/* "insertion" */
			insertion(Mot);
			printf("%s inser�.\n",Mot);
			break;
		case 's':	/* "suppression" */
			if (suppression(Mot))
				printf("%s supprim�.\n",Mot);
			else
				printf("%s non trouv�.\n",Mot);
			break;
		case 'c':	/* "cherche" */
			if (cherche(Mot))
				printf("%s trouv�.\n",Mot);
			else
				printf("%s non trouv�.\n",Mot);
			break;
		case 'q':	/* quit */
			printf("fin de session.\n");
			exit(0);
		default:	/* entr�e erron�e */
			printf("commande %c invalide (I)nitialiser,(i)ns�rer,(s)upprimer,(c)chercher,(q)uitter).\n", Commande);
			break;
		}
	}
}

/*------------------------------------------------------------------------
 * CommandeSuivante  - lit une commande et (�ventuellement) un mot
 *------------------------------------------------------------------------
 */
int 
CommandeSuivante(Commande, Mot)
char *Commande, *Mot;
{
  printf("Commande: ");
  scanf("%s",Commande);
  /* printf("La commande saisie est : %c \n",*Commande); */
  if (*Commande!='q' && *Commande!='I'){
    printf("Mot: ");
    /* voir l'utilisation de la constante TAILLEMOT dans le format */
    scanf("%50s",Mot);
    /* printf("Le mot saisi est : %s (%d)\n",Mot,strlen(Mot)); */
    return(strlen(Mot));
  } 
  else return(-1);
}
