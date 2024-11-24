#ifndef PERSISTANCE_H
#define PERSISTANCE_H

#define CLIENTS_FILE "./data/clients.json"

#include "cJSON.h"
#include "stdio.h"
#include <stdlib.h>

char *read_file(const char *filename);
int write_file(const char *filename, const char *content);
int add_name_to_db(const char *name);

#endif