#include "game.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Debug function to print the game data
 * @param game The game to print
 */
void print_game_infos(Game *game) {

  printf("\nNombre de joueurs : %d\n", game->n_player);
  // printf("Nombre de tours : %d\n", game->n_turn);
  printf("Nombre de penguins par joueurs : %d\n", game->n_ping_per_player);

  // players data
  printf("Liste des joueurs/joueuses : \n");
  for (unsigned int i = 0; i < game->n_player; i++) {
    printf("%s :\n", game->player_tab[i].name);
    printf("\tscore : %d\n", game->player_tab[i].score);

    printf("\tpenguins : \n");
    for (unsigned int j = 0; j < (game->n_ping_per_player); j++) {
      printf("\t\tn%d x = %d", j, game->player_tab[i].penguins[j].x);
      printf("  y = %d\n", game->player_tab[i].penguins[j].y);
    }
  }
  // TODO print debug map
}

void print_map(Game *game, char map_str[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH]) {
  assert(game != NULL && map_str != NULL);
  for (int y = 0, screen_y = 0; y < N_ROWS; y++, screen_y += TILE_HEIGHT) {
    for (int x = 0, screen_x = 0; x < N_COLS; x++, screen_x += TILE_WIDTH) {
      if (x % 2 == 0) { // height variation : tiles that are lower are on even x
        screen_y += 2;
      } else {
        screen_y -= 2;
      }
      print_tile(map_str, screen_y, screen_x);
    }
  }
  clear();
  for (int screen_y = 0; screen_y < MAX_SCREEN_HEIGHT; screen_y++) {
    printf("%s\n", map_str[screen_y]);
  }
}

/**
 * @brief Verify if the game is valid exit if not (redondant)
 * @param game to verify
 * @return true if the game is valid
 */
bool game_is_valid(Game *game) {
  // testing general game data
  if (game == NULL) {
    printf("pointeur jeu nul\n");
    return false;
  }
  if (game->player_tab == NULL) {
    printf("pointeur joueurs nul\n");
    return false;
  }
  if (game->n_player < N_MIN_PLAYERS || game->n_player > N_MAX_PLAYERS) {
    printf("erreur : nombre de joueurs incorrect (%d)\n", game->n_player);
    return false;
  }
  if (game->n_ping_per_player < N_MIN_PENGUINS_PER_PLAYER ||
      game->n_ping_per_player > N_MAX_PENGUINS_PER_PLAYER) {
    printf("n_ping_per_player incorrect\n");
    return false;
  }
  // testing players data
  for (int i = 0; i < game->n_player; i++) {
    if (!player_is_valid(game->player_tab[i], game->n_ping_per_player)) {
      printf("joueur %d incorrect", i);
      return false;
    }
  }
  // testing map data
  if (!map_is_valid(&(game->map))) {
    printf("map is not valid\n");
    return false;
  }

  return true;
}

bool tile_has_penguin(Game *game, unsigned int x, unsigned int y) {
  assert(game_is_valid(game));
  for (unsigned int i = 0; i < game->n_player; i++) {
    for (unsigned int j = 0; j < game->n_ping_per_player; j++) {
      if (game->player_tab[i].penguins[j].x == x &&
          game->player_tab[i].penguins[j].y == y) {
        return true;
      }
    }
  }
  return false;
}

/**
 * @brief Initialise the game with player input
 * @param *game game to initialise
 */
void init_game(Game *game) {

  game->n_player =
      val_between(N_MIN_PLAYERS, N_MAX_PLAYERS, "nombre de joueurs");
  assert(game->n_player >= N_MIN_PLAYERS && game->n_player <= N_MAX_PLAYERS);
  game->n_ping_per_player = 6 - game->n_player;
  assert(game->n_ping_per_player >= N_MIN_PENGUINS_PER_PLAYER &&
         game->n_ping_per_player <= N_MAX_PENGUINS_PER_PLAYER);

  game->player_tab = malloc(game->n_player * sizeof(Player));
  verify(game->player_tab != NULL, "erreur d'allocation memoire");
  init_player_tab(game->player_tab, game->n_player, game->n_ping_per_player);

  game->map = map_new();
  printf("map is valid 2 : %d\n\n", map_is_valid(&(game->map)));
  finish_starter_map(game);
  //    TO DO : init player pos
}

/**
 * @brief make sure that at the beggining of the game, the map has enough case
 * with one fish of value 1
 * @param *game game to finish initialising
 */
void finish_starter_map(Game *game) {

  assert(game_is_valid(game));

  // count number of case with one fish
  int nb_1_fish = 0;
  for (int y = 0; y < N_ROWS; y++) {
    for (int x = 0; x < N_COLS; x++) {

      assert(tile_correctly_init(game->map.tiles[y][x]));
      if (game->map.tiles[y][x].n_fishes == 1) {
        if (game->map.tiles[y][x].fishValues[0] == 1) {
          nb_1_fish++;
        }
      }
    }
  }

  unsigned int nb_penguins = game->n_player * game->n_ping_per_player;
  int nx, ny;
  // make sure that at the beggining of the game, the map has enough case with
  // one fish of value 1
  while (nb_penguins > nb_1_fish) // dis is veri efectiv
  {
    nx = rand() % N_COLS;
    ny = rand() % N_ROWS;
    if (game->map.tiles[ny][nx].n_fishes != 1 ||
        game->map.tiles[ny][nx].fishValues[0] != 1) {
      game->map.tiles[ny][nx].n_fishes = 1;
      game->map.tiles[ny][nx].fishValues[0] = 1;
      nb_1_fish++;
    }
  }
}

/**
 * @brief Update game data with the movement of a penguin
 * @param *game game to update
 * @param play_turn player who play
 * @param i_pin_to_move index of the penguin to move
 * @param nx new x position of the penguin
 * @param ny new y position of the penguin
 */
void update_game_data(Game *game, unsigned int play_turn,
                      unsigned int i_pin_to_move, unsigned int nx,
                      unsigned int ny) {
  // asserts
  assert(coordinate_are_valid(nx, ny));
  assert(play_turn < game->n_player);
  assert(i_pin_to_move < game->n_ping_per_player);
  assert(game_is_valid(game));

  // update penguin pos
  int x = game->player_tab[play_turn].penguins[i_pin_to_move].x;
  int y = game->player_tab[play_turn].penguins[i_pin_to_move].y;
  game->player_tab[play_turn].penguins[i_pin_to_move].x = nx;
  game->player_tab[play_turn].penguins[i_pin_to_move].y = ny;

  // uptdate player score (variante FISH_GOLDEN) :
  for (int i = 0; i < game->map.tiles[y][x].n_fishes; i++) {
    game->player_tab[play_turn].score += game->map.tiles[ny][nx].fishValues[i];
  }

  // update map
  game->map.tiles[ny][nx].n_fishes = 0;
  game->map.tiles[ny][nx].is_usable = 0;
}
//*/