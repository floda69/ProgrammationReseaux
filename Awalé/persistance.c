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

int is_string_in_json_array(cJSON *array, const char *string)
{
   // pas besoin de chercher
   if (!cJSON_IsArray(array) || string == NULL)
   {
      return 0;
   }

   cJSON *item = NULL;
   cJSON_ArrayForEach(item, array)
   {
      if (cJSON_IsString(item) && strcmp(item->valuestring, string) == 0)
      {
         return 1;
      }
   }
   return 0;
}

int delete_bio_from_db(cJSON *array, const char *name)
{
   // pas besoin de chercher
   if (!cJSON_IsArray(array) || name == NULL)
   {
      return 0;
   }

   int index = 0;
   cJSON *item = NULL;
   cJSON_ArrayForEach(item, array)
   {
      cJSON *name_item = cJSON_GetObjectItem(item, "name");
      if (cJSON_IsString(name_item) && strcmp(name_item->valuestring, name) == 0)
      {
         // Supprimer l'objet correspondant
         cJSON_DeleteItemFromArray(array, index);
         return 1;
      }
      index++;
   }
   return 0;
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

   // ajouter nom à la liste des clients SSI il n'y est pas
   cJSON *clients = cJSON_GetObjectItem(json, "clients");
   if (is_string_in_json_array(clients, name))
      return 0;
   cJSON *name_json = cJSON_CreateString(name);
   cJSON_AddItemToArray(clients, name_json);

   // convertir le JSON mis à jour en chaîne
   char *updated_json = cJSON_Print(json);

   // écrire le JSON mis à jour dans le fichier
   if (!write_file(CLIENTS_FILE, updated_json))
   {
      perror("Erreur lors de l'écriture du fichier\n");
      free(updated_json);
      cJSON_Delete(json);
      return 0;
   }

   // !!! nettoyage
   free(updated_json);
   cJSON_Delete(json);

   return 1;
}

int change_bio_in_db(const char *name, const char *new_bio)
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

   // créer objet bio
   cJSON *bio_entry = cJSON_CreateObject();
   cJSON_AddStringToObject(bio_entry, "name", name);
   cJSON_AddStringToObject(bio_entry, "bio", new_bio);
   cJSON *bios = cJSON_GetObjectItem(json, "bios");

   // supprimer ancienne bio si existait
   delete_bio_from_db(bios, name);

   // ajouter nouvelle bio
   cJSON_AddItemToArray(bios, bio_entry);

   // convertir le JSON mis à jour en chaîne
   char *updated_json = cJSON_Print(json);

   // écrire le JSON mis à jour dans le fichier
   if (!write_file(CLIENTS_FILE, updated_json))
   {
      perror("Erreur lors de l'écriture du fichier\n");
      free(updated_json);
      cJSON_Delete(json);
      return 0;
   }

   // !!! nettoyage
   free(updated_json);
   cJSON_Delete(json);

   return 1;
}

char *get_bio_from_db(const char *name, char *bio_buffer)
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

   cJSON *bios = cJSON_GetObjectItem(json, "bios");

   cJSON *item = NULL;
   // pour chaque entré du bios on vérifie si le nom est trouvé
   cJSON_ArrayForEach(item, bios)
   {
      cJSON *name_item = cJSON_GetObjectItem(item, "name");
      if (cJSON_IsString(name_item) && strcmp(name_item->valuestring, name) == 0)
      {
         // renvoyer la bio si trouvée
         strncpy(bio_buffer, cJSON_GetObjectItem(item, "bio")->valuestring, BIO_SIZE - 1);
      }
   }
   
   cJSON_Delete(json);

   return bio_buffer;
}