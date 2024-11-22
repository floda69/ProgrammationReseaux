#ifndef SERVER_H
#define SERVER_H

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

#define CRLF        "\r\n"
#define PORT         1977
#define MAX_CLIENTS     100

#define BUF_SIZE    1024
#define NAME_SIZE    50

#include "client.h"
#include "../message.h"
#include "../jeu.h"
#include <string.h>

static void init(void);
static void end(void);
static void app(void);
static int init_connection(void);
static void end_connection(int sock);
static int read_client(SOCKET sock, char *buffer);
static void write_client(SOCKET sock, const char *buffer);
static void send_message_to_all_clients(Client *clients, Client client, int actual, const char *buffer, char from_server);
static void send_message_to_client(Client receiver, const char *buffer);
static void send_clients_list_on_demand(Client *clients, Client client, int actual);
static void switch_client_chat_mode(Client *client);
static void defy_player(Client *clients, Client defier, int actual, const char *playerDefied);
static void remove_client(Client *clients, int to_remove, int *actual);
static void clear_clients(Client *clients, int actual);
static int get_player_index_by_name(Client *clients, char *name, int actual);
static void decline_invite(Client *clients, Client client, int actual);
static void accept_invite(Client *clients, Client client, int actual);
static void launch_game(Client *clients, Client client, int actual, Awale *games, int *gameIndex);
static void send_game(Awale *game, Client* clients, int actual);
static int get_game_index_by_name(Awale *games, char *name, int actual);
static void play(int case_choisie, Client *clients, Client client, int actual, Awale *games, int *gameIndex);
static void end_game(Awale* games, int gameIndex, Awale *game, int index_game, Client *clients, int actual);

#endif /* guard */
