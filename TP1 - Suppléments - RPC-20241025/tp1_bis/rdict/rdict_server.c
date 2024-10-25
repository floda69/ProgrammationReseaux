/*
 * 
 * Code intermédiaire d'après rpcgen
 * 
 */

#include "rdict.h"

int* initialise_1_svc(struct svc_req *rqstp)
{
	static int  resultat;

	/*
	 * insert server code here
	 */
	resultat = initialise();

	return (&resultat);
}

int* insertion_1_svc(char *argp, struct svc_req *rqstp)
{
	static int  resultat;

	/*
	 * insert server code here
	 */
	resultat = insertion(argp);
	return(&resultat);
}

int * suppression_1_svc(char * argp, struct svc_req *rqstp)
{
	static int  resultat;

	/*
	 * insert server code here
	 */
	resultat = suppression(argp);

	return(&resultat);
}

int * cherche_1_svc(char *argp, struct svc_req *rqstp)
{
	static int  resultat;

	/*
	 * insert server code here
	 */
	resultat = cherche(argp);
	return(&resultat);
}
