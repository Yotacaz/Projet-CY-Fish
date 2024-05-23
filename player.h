#include "penguin.h"

#define N_MIN_PLAYERS 2
#define N_MAX_PLAYERS 10

#define N_MIN_PENGUINS_PER_PLAYER 2
#define N_MAX_PENGUINS_PER_PLAYER 4

#define MIN_NAME_LEN 1
#define MAX_NAME_LEN 10

typedef struct {
  Penguin *penguins;
  int score;
  char *name;
} Player;

_Bool player_is_valid(Player player, unsigned int nb_penguin_per_player);

void init_player_tab(Player *player_tab, int n_player, int n_ping_per_player);

Penguin *create_penguin_array();