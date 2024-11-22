#include "message.h"


const char* serialize_message(char *code, const char *message){
    char *s = malloc(strlen(code)+strlen(message)+1);
    strcpy(s, code);
    return(strcat(s, message));
}

void serialize_awale(Awale *jeu, char *buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size,
            "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"     // plateau
            "%d,%d,"                                   // score
            "%s,%s,"                                   // joueurs
            "%d",                                      // turn
             jeu->plateau[0][0], jeu->plateau[0][1], jeu->plateau[0][2], jeu->plateau[0][3], jeu->plateau[0][4], jeu->plateau[0][5],
             jeu->plateau[1][0], jeu->plateau[1][1], jeu->plateau[1][2], jeu->plateau[1][3], jeu->plateau[1][4], jeu->plateau[1][5],
             jeu->score[0], jeu->score[1],
             jeu->j1, jeu->j2,
             jeu->turn);
}

void deserialize_awale(Awale *jeu, const char *buffer) {
    sscanf(buffer,
           "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"     // plateau
           "%d,%d,"                                   // score
           "%49[^,],%49[^,],"                         // joueurs
           "%d",                                      // turn
           &jeu->plateau[0][0], &jeu->plateau[0][1], &jeu->plateau[0][2], &jeu->plateau[0][3], &jeu->plateau[0][4], &jeu->plateau[0][5],
           &jeu->plateau[1][0], &jeu->plateau[1][1], &jeu->plateau[1][2], &jeu->plateau[1][3], &jeu->plateau[1][4], &jeu->plateau[1][5],
           &jeu->score[0], &jeu->score[1],
           jeu->j1, jeu->j2,
           &jeu->turn);
}