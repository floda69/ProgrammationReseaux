/* rdict_srp.c - initialise, insertion, suppression, cherche */

#include <rpc/rpc.h>
#include <stdio.h>
#include "rdict.h"

/* proc�dures distantes cot� serveur avec leurs donn�es */

char Dictionnaire[TAILLEDICTIONNAIRE][TAILLEMOT+1];/* dictionnaire de mots */
int  CompteurMots = 0;	/* nombre de mots dans le dictionnaire */

/*------------------------------------------------------------------------
 * initialise - initialise le dictionnaire avec 0 mots
 *------------------------------------------------------------------------
 */
int
initialise()
{
	CompteurMots = 0;
        printf("Initialisation du dictionnaire ...\n");
	return 1;
}

/*------------------------------------------------------------------------
 * insertion - ins�re un mot dans le dictionnaire
 *------------------------------------------------------------------------
 */
int
insertion(Mot)
char	*Mot;
{
	strcpy(Dictionnaire[CompteurMots], Mot);
	CompteurMots++;
	return CompteurMots;
}

/*------------------------------------------------------------------------
 * suppression - supprime un mot du dictionnaire
 *------------------------------------------------------------------------
 */
int
suppression(Mot)
char	*Mot;
{
	int	i;

	for (i=0 ; i<CompteurMots ; i++)
		if (strcmp(Mot, Dictionnaire[i]) == 0) {
			CompteurMots--;
			strcpy(Dictionnaire[i], Dictionnaire[CompteurMots]);
			return 1;
		}
	return 0;
}

/*------------------------------------------------------------------------
 * cherche - cherche un mot dans le dictionnaire
 *------------------------------------------------------------------------
 */
int
cherche(Mot)
char	*Mot;
{
	int	i;

	for (i=0 ; i<CompteurMots; i++)
		if (strcmp(Mot, Dictionnaire[i]) == 0)
			return 1;
	return 0;
}
