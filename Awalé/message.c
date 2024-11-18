#include "message.h"


const char* serialize_message(char *code, const char *message){
    char *s = malloc(strlen(code)+strlen(message)+1);
    strcpy(s, code);
    return(strcat(s, message));
}

void serialize_coup(Awale *jeu, char *buffer) {
    
    size_t offset = 0;

    // Copier plateau
    memcpy(buffer + offset, jeu->plateau, sizeof(jeu->plateau));
    offset += sizeof(jeu->plateau);
    buffer[offset++] = '\n';

    // Copier score
    memcpy(buffer + offset, jeu->score, sizeof(jeu->score));
    offset += sizeof(jeu->score);
    buffer[offset++] = '\n';

    // Copier j1
    memcpy(buffer + offset, jeu->j1, sizeof(jeu->j1));
    offset += sizeof(jeu->j1);
    buffer[offset++] = '\n';

    // Copier j2
    memcpy(buffer + offset, jeu->j2, sizeof(jeu->j2));
    offset += sizeof(jeu->j2);
    buffer[offset++] = '\n';
}


void deserialize_coup(Awale *jeu, char *buffer) {
    size_t offset = 0;

    // Copier plateau
    memcpy(jeu->plateau, buffer + offset, sizeof(jeu->plateau));
    offset += sizeof(jeu->plateau);
    offset++; // Skip the newline character

    // Copier score
    memcpy(jeu->score, buffer + offset, sizeof(jeu->score));
    offset += sizeof(jeu->score);
    offset++; // Skip the newline character

    // Copier j1
    memcpy(jeu->j1, buffer + offset, sizeof(jeu->j1));
    offset += sizeof(jeu->j1);
    offset++; // Skip the newline character

    // Copier j2
    memcpy(jeu->j2, buffer + offset, sizeof(jeu->j2));
    offset += sizeof(jeu->j2);
    offset++; // Skip the newline character
}

void serialize_awale(Awale *jeu, char *buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size,
             "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d," // plateau
             "%d,%d,"                                // score
             "%s,%s",                                // joueurs
             jeu->plateau[0][0], jeu->plateau[0][1], jeu->plateau[0][2], jeu->plateau[0][3], jeu->plateau[0][4], jeu->plateau[0][5],
             jeu->plateau[1][0], jeu->plateau[1][1], jeu->plateau[1][2], jeu->plateau[1][3], jeu->plateau[1][4], jeu->plateau[1][5],
             jeu->score[0], jeu->score[1],
             jeu->j1, jeu->j2);
}

void deserialize_awale(const char *buffer, Awale *jeu) {
    sscanf(buffer,
           "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
           "%d,%d,"
           "%s,%s",
           &jeu->plateau[0][0], &jeu->plateau[0][1], &jeu->plateau[0][2], &jeu->plateau[0][3], &jeu->plateau[0][4], &jeu->plateau[0][5],
           &jeu->plateau[1][0], &jeu->plateau[1][1], &jeu->plateau[1][2], &jeu->plateau[1][3], &jeu->plateau[1][4], &jeu->plateau[1][5],
           &jeu->score[0], &jeu->score[1],
           jeu->j1, jeu->j2);
}