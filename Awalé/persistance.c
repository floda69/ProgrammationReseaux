#include "persistance.h"
// attention penser à free le résultat car malloc
char *read_file(const char *filename)
{
   FILE *file = fopen(filename, "r");
   if (file == NULL)
   {
      perror("Erreur d'ouverture du fichier");
      return NULL;
   }
   fseek(file, 0, SEEK_END);
   long length = ftell(file);
   fseek(file, 0, SEEK_SET);

   char *content = malloc(length + 1);
   if (content == NULL)
   {
      perror("Erreur d'allocation mémoire");
      fclose(file);
      return NULL;
   }
   fread(content, 1, length, file);
   fclose(file);
   content[length] = '\0';
   return content;
}

int write_file(const char *filename, const char *content)
{
   FILE *file = fopen(filename, "w");
   if (file == NULL)
   {
      perror("Erreur d'écriture dans le fichier");
      return 0;
   }
   fprintf(file, "%s", content);
   fclose(file);
   return 1;
}

int add_name_to_db(const char *name)
{
   char *json_clients = read_file(CLIENTS_FILE);
   if (json_clients == NULL)
   {
      return 0;
   }
   // parser string vers objet JSON
   cJSON *json = cJSON_Parse(json_clients);
   free(json_clients);
   if (json == NULL)
   {
      perror("Erreur lors du parsing du JSON\n");
      return 0;
   }

   // ajouter nom à la liste des clients
   cJSON *clients = cJSON_GetObjectItem(json, "clients");
   cJSON *name_json = cJSON_CreateString(name);
   cJSON_AddItemToArray(clients, name_json);

   // convertir le JSON mis à jour en chaîne
   char *updated_json = cJSON_Print(json);

   // Étape 6 : Écrire le JSON mis à jour dans le fichier
   if (!write_file(CLIENTS_FILE, updated_json))
   {
      perror("Erreur lors de l'écriture du fichier\n");
      free(updated_json);
      cJSON_Delete(json);
      return 0;
   }

   // Nettoyage
   free(updated_json);
   cJSON_Delete(json);

   return 1;
}