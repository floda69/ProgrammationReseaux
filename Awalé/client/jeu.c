#include <stdio.h>
#include <stdbool.h>
#include "jeu.h"

// Initialisation du jeu
void initialiser_jeu(Awale *jeu)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < NBR_CASES; j++)
        {
            jeu->plateau[i][j] = GRAINES_PAR_CASE;
        }
        jeu->score[i] = 0;
    }
}

// Affichage du plateau
void afficher_plateau(Awale *jeu)
{
    printf("Plateau :\n");
    int i = 0;
    while (jeu->j1[i] != '\0')
    {
        printf("%c", jeu->j1[i]);
        ++i;
    }
    printf("\n\t");
    for (int j = 0; j < NBR_CASES; j++)
    {
        printf("%2d ", jeu->plateau[0][j]);
    }
    printf("\n");

    printf(" %2d\t\t\t\t%2d\n", jeu->score[0], jeu->score[1]);

    printf("\t");
    for (int j = 0; j < NBR_CASES; j++)
    {
        printf("%2d ", jeu->plateau[1][j]);
    }
    printf("\n\t\t\t\t");
    i = 0;
    while (jeu->j2[i] != '\0')
    {
        printf("%c", jeu->j2[i]);
        ++i;
    }
    printf("\n");
}

// Vérification si un coup est valide
bool coup_valide(Awale *jeu, int joueur, int case_choisie)
{
    if (case_choisie < 0 || case_choisie >= NBR_CASES)
        return false;
    if (jeu->plateau[joueur][case_choisie] == 0)
        return false;
    return true;
}
// test si la capture est possible
bool capture_possible(Awale *jeu, int rang, int case_actuelle)
{
    if (jeu->plateau[rang][case_actuelle] == 2 || jeu->plateau[rang][case_actuelle] == 3)
        return true;
    else
        return false;
}

// test de la situation de faim
bool pas_de_famine(Awale *jeu, int joueur, int case_actuelle)
{
    // si J1 famine SSI toutes cases adverses à 2 ou 3 et prise en case 5
    if (joueur == J1 && case_actuelle == 5)
    {
        for (int i = 0; i < NBR_CASES; i++)
            if (jeu->plateau[J2][i] != 2 && jeu->plateau[J2][i] != 3)
                return true;
        return false;
    }
    // si J2 famine SSI toutes cases adverses à 2 ou 3 et prise en case 0
    else if (joueur == J2 && case_actuelle == 0)
    {
        for (int i = 0; i < NBR_CASES; i++)
            if (jeu->plateau[J1][i] != 2 && jeu->plateau[J1][i] != 3)
                return true;
        return false;
    }
}

// Exécution d'un coup
void jouer_coup(Awale *jeu, int joueur, int case_choisie)
{
    int graines = jeu->plateau[joueur][case_choisie];
    jeu->plateau[joueur][case_choisie] = 0;

    int rang = joueur;
    int case_actuelle = case_choisie;
    while (graines > 0)
    {
        case_actuelle += (rang == 0) ? -1 : 1;

        if (case_actuelle < 0 || case_actuelle >= NBR_CASES)
        {
            rang = 1 - rang;
            case_actuelle = (rang == 0) ? NBR_CASES - 1 : 0;
        }

        jeu->plateau[rang][case_actuelle]++;
        graines--;
    }

    // Capture de graines si dans la rangée adverse et condition de capture
    if (rang != joueur && pas_de_famine(jeu, joueur, case_actuelle) && capture_possible(jeu, rang, case_actuelle))
    {
        int capture = 0;
        while (case_actuelle >= 0 && capture_possible(jeu, rang, case_actuelle))
        {
            capture += jeu->plateau[rang][case_actuelle];
            jeu->plateau[rang][case_actuelle] = 0;
            case_actuelle--;
        }
        jeu->score[joueur] += capture;
    }
}

// Vérification de fin de jeu
bool fin_de_jeu(Awale *jeu)
{
    int total[2] = {0, 0};
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < NBR_CASES; j++)
        {
            total[i] += jeu->plateau[i][j];
        }
    }
    return (total[0] == 0 || total[1] == 0);
}

// Fonction principale
int game()
{
    Awale jeu = {.j1 = "toto", .j2 = "tutu"};
    initialiser_jeu(&jeu);

    int joueur = 0; // 0 pour le joueur 1, 1 pour le joueur 2
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

    return 0;
}