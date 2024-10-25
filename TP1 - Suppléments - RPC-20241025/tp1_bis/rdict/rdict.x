/* rdict.x */

/* Declarations en RPCL du programme dictionnaire pour l'outil rpcgen */

const TAILLEMOT = 50;           /* Taille maximale d'un mot du dictionnaire */
const TAILLEDICTIONNAIRE = 100; /* nombre d'entr�es du dictionnaire */

struct example {                /* unused structure declared here to    */
        int     exfield1;       /* illustrate how rpcgen builds XDR     */
        char    exfield2;       /* routines to convert structures.      */
};

/*------------------------------------------------------------------------
 * RDICTPROG - programme distant qui propose les services de gestion
 * du dictionnaire : initialise, insertion, suppression, cherche
 * Les num�ros sont ceux utilis�s par portmap
 * voir rpcinfo -p localhost (les num�ros sont en d�cimal)
 *------------------------------------------------------------------------
 */
program RDICTPROG {		    /* nom du programme                  */
    version RDICTVERS {		    /* num�ro de version                 */
	int INITIALISE(void)  = 1;  /* premi�re proc�dure du programme   */ 
	int INSERTION(string) = 2;  /* seconde proc�dure                 */
	int SUPPRESSION(string) = 3;/* troisi�me proc�dure               */
	int CHERCHE(string) = 4;    /* quatri�me proc�dure               */
    } = 1;			    /* num�ro de la version du programme */
} = 0x30090949;			    /* num�ro de programme               */

