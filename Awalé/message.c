#include "message.h"

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