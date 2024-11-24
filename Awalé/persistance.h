#ifndef PERSISTANCE_H
#define PERSISTANCE_H

#define CLIENTS_FILE "./data/clients.json"
#define BIO_SIZE    200

#include "cJSON.h"
#include "stdio.h"
#include <stdlib.h>

char *read_file(const char *filename);
int write_file(const char *filename, const char *content);
int is_string_in_json_array(cJSON *array, const char *string);
int delete_bio_from_db(cJSON *array, const char *name);
int has_friends(const char *name, cJSON *array);
int is_in_private_mode(const char *name);
int add_name_to_db(const char *name);
int change_bio_in_db(const char *name, const char *new_bio);
char *get_bio_from_db(const char *name, char *bio_buffer);
int add_friend_to_db(const char *name, const char *friend);
int switch_privacy_in_db(const char *name);

#endif