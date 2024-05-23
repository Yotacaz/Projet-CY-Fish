#include "player.h"
#include "penguin.h"
#include "utils.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief Verify if the player is valid exit if not (redondant)
 * @param player player to verify
 * @param n_ping_per_player	number of penguins to verify for the player
 * @return true if the player is valid
 */
bool player_is_valid(Player player, unsigned int n_ping_per_player) {
  // general datas
  if (player.score < 0 || n_ping_per_player < 2 || n_ping_per_player > 5) {
    return false;
  } else if (player.penguins == NULL) {
    return false;
  }
  // penguin datas
  for (int i = 0; i < n_ping_per_player; i++) {
    if (penguin_is_valid(player.penguins + i) == false) {
      return false;
    }
  }
  return true;
}

/**
 * @brief Initialise the player tab
 * @param *player_tab to initialise
 * @param n_player	number of player
 */
void init_player_tab(Player *player_tab, int n_player, int n_ping_per_player) {
  assert(player_tab != NULL);
  assert(n_player >= N_MIN_PLAYERS && n_player <= N_MAX_PLAYERS);
  assert(n_ping_per_player >= N_MIN_PENGUINS_PER_PLAYER &&
         n_ping_per_player <= N_MAX_PENGUINS_PER_PLAYER);

	//players name + general data init
  bool name_is_ok = false;
  for (int i = 0; i < n_player; i++) {
    player_tab[i].score = 0;
    do {
      printf("\njoueur/joueuse %d :\n", i + 1);
      name_is_ok = true;
      player_tab[i].name = scan_str(MAX_NAME_LEN, "\tNom");

		//checking if the name is long enougth
      if (strlen(player_tab[i].name) < MIN_NAME_LEN) {
        printf(RED "Erreur : le nom du joueur doit contenir au moins %d "
                   "characteres\n" RESET,
               MIN_NAME_LEN);
        name_is_ok = false;
      } else {
		  //checking if name is already taken
        for (int j = 0; j < i; j++) {
          if (strcmp(player_tab[i].name, player_tab[j].name) == 0) {
            printf(RED "Erreur : nom de joueur deja utilise\n" RESET);
            name_is_ok = false;
          }
        }
      }

      // initialising the penguins tab
      player_tab[i].penguins = malloc(sizeof(Penguin) * n_ping_per_player);
      verify(player_tab[i].penguins != NULL, "erreur d'allocation memoire");
      for (int j = 0; j < n_ping_per_player; j++) {
        // TODO after init_peng is done :  initialise at -1 for testing
        // (-1=2^32)
        player_tab[i].penguins[j].x = 0;
        player_tab[i].penguins[j].y = 0;
      }

    } while (name_is_ok == false);
  }
}