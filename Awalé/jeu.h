#ifndef JEU_H
#define JEU_H

#define NBR_CASES 6
#define GRAINES_PAR_CASE 4
#define J1 0
#define J2 1

#include <stdbool.h>

typedef struct {
    int plateau[2][NBR_CASES]; // 2 rangées de 6 cases
    int score[2];              // Scores des deux joueurs
    char j1[50];
    char j2[50];
    int turn;
} Awale;

// Initialisation du jeu
void initialiser_jeu(Awale *jeu);

// Affichage du plateau
void afficher_plateau(Awale *jeu);

// Vérification si un coup est valide
bool coup_valide(Awale *jeu, int joueur, int case_choisie);

// test si la capture est possible 
bool capture_possible(Awale *jeu, int rang, int case_actuelle);

// test de la situation de faim
bool pas_de_famine(Awale *jeu, int joueur, int case_actuelle);

// Exécution d'un coup
void jouer_coup(Awale *jeu, int joueur, int case_choisie);

// Vérification de fin de jeu
bool fin_de_jeu(Awale *jeu);

// Fonction principale
int game();

#endif// Structure pour le jeu d'Awalé