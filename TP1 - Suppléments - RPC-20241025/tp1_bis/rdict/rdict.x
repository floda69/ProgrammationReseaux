/* rdict.x */

/* Declarations en RPCL du programme dictionnaire pour l'outil rpcgen */

const TAILLEMOT = 50;           /* Taille maximale d'un mot du dictionnaire */
const TAILLEDICTIONNAIRE = 100; /* nombre d'entrées du dictionnaire */

struct example {                /* unused structure declared here to    */
        int     exfield1;       /* illustrate how rpcgen builds XDR     */
        char    exfield2;       /* routines to convert structures.      */
};

/*------------------------------------------------------------------------
 * RDICTPROG - programme distant qui propose les services de gestion
 * du dictionnaire : initialise, insertion, suppression, cherche
 * Les numéros sont ceux utilisés par portmap
 * voir rpcinfo -p localhost (les numéros sont en décimal)
 *------------------------------------------------------------------------
 */
program RDICTPROG {		    /* nom du programme                  */
    version RDICTVERS {		    /* numéro de version                 */
	int INITIALISE(void)  = 1;  /* premiére procédure du programme   */ 
	int INSERTION(string) = 2;  /* seconde procédure                 */
	int SUPPRESSION(string) = 3;/* troisiéme procédure               */
	int CHERCHE(string) = 4;    /* quatriéme procédure               */
    } = 1;			    /* numéro de la version du programme */
} = 0x30090949;			    /* numéro de programme               */

