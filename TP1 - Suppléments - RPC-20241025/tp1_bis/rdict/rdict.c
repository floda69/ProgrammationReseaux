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
/* Le client interroge un dictionnaire (implanté sous forme de tableau)
   résidant sur la machine serveur.

   Les commandes possibles sont: I pour initialiser à zéro le dictionnaire
                                 i pour insérer un mot
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
   Dictionnaire initialisé à vide.
   Commande: i
   Mot: Robert
   Robert inseré.
   Commande: i
   Mot: Jules
   Jules inseré.
   Commande: l
   Mot: toto
   commande l invalide (entrez I,s,c,q).
   Commande: c
   Mot: toto
   toto non trouvé.
   Commande: c
   Mot: Jules
   Jules trouvé.
   Commande: s
   Mot: Jules
   Jules supprimé.
   Commande: c
   Mot: Jules
   Jules non trouvé.
   Commande: c
   Mot: Robert
   Robert trouvé.
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
                                /* à passer en paramètre argv[1] */
CLIENT	*handle;		/* handle de la procédure serveur */

/*------------------------------------------------------------------------
 * main - insere, supprime, ou cherche un mot dans le dictionnaire
 *------------------------------------------------------------------------
 */
int main(int argc, char* argv[])
{
	char	Mot[TAILLEMOT+1];  /* allocation de la mémoire pour un mot */
	char	Commande;          /* commande saisie */
	int	LongueurMot;	   /* longueur réelle du mot saisi */

      /* connexion avec le serveur pour les appels RPC  */
	
       handle = clnt_create(argv[1], RDICTPROG, RDICTVERS, "tcp");
       if (handle == 0) {
               printf("La connexion avec le programme distant ne peut etre établie...\n");
               exit(1);
       }

       while (1) {
	        printf("\n\n(I)nitialiser,(i)nsérer,(s)upprimer,(c)chercher,(q)uitter\n");
	        LongueurMot = CommandeSuivante(&Commande, Mot);

		switch (Commande) {
		case 'I':	/* "initialise" */
			initialise();
			printf("Dictionnaire initialisé à vide.\n");
			break;

		case 'i':	/* "insertion" */
			insertion(Mot);
			printf("%s inseré.\n",Mot);
			break;
		case 's':	/* "suppression" */
			if (suppression(Mot))
				printf("%s supprimé.\n",Mot);
			else
				printf("%s non trouvé.\n",Mot);
			break;
		case 'c':	/* "cherche" */
			if (cherche(Mot))
				printf("%s trouvé.\n",Mot);
			else
				printf("%s non trouvé.\n",Mot);
			break;
		case 'q':	/* quit */
			printf("fin de session.\n");
			exit(0);
		default:	/* entrée erronée */
			printf("commande %c invalide (I)nitialiser,(i)nsérer,(s)upprimer,(c)chercher,(q)uitter).\n", Commande);
			break;
		}
	}
}

/*------------------------------------------------------------------------
 * CommandeSuivante  - lit une commande et (éventuellement) un mot
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
