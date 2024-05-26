#include "game.h"
#include "bmp_img.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// PRINT FUNCTIONS
/**
 * @brief function to print various the game data at the right of the screen
 * @param game The game to print
 */
void print_game_infos(Game *game) {
  assert(game_is_valid(game));
  unsigned int x = MAX_MAP_SCREEN_WIDTH + 4;
  print_at_xy(x, 1, " _______________________");
  print_at_xy(x, 2, "| " BOLD_MAG "CY-FISH" RESET " (tour %d) ", game->n_turn);
  print_at_xy(x, 3, "| %d pingouins par joueurs", game->n_peng_per_player);
  print_at_xy(x, 4, "¦_______________________");
  print_at_xy(x, 5, "| " BOLD_MAG "joueurs/joueuses" MAG "(%d):" RESET,
              game->n_player);
  for (unsigned int i = 0; i < game->n_player; i++) {
    print_at_xy(x, 6 + i, "| \x1B[%dm%s (%d poissons)" RESET, i + 41,
                game->player_tab[i].name, game->player_tab[i].score);
  }
  print_at_xy(x, 6 + game->n_player, "¦_______________________");
  print_at_xy(x, 7 + game->n_player,
              "| " BOLD_MAG "Valeur des poissons " RESET ":");
  print_at_xy(x, 8 + game->n_player, "|\t🐟 = 1 pt");
  print_at_xy(x, 9 + game->n_player, "|\t🐠 = 2 pt");
  print_at_xy(x, 10 + game->n_player, "|\t🐡 = 3 pt");
  print_at_xy(x, 11 + game->n_player, "¦_______________________");
  print_at_xy(x, 12 + game->n_player,
              "| " BOLD_MAG "Schéma des déplacements" RESET);
  print_at_xy(x, 13 + game->n_player, "|       6");
  print_at_xy(x, 14 + game->n_player, "|     _____  ");
  print_at_xy(x, 15 + game->n_player, "| 5  /     \\  1");
  print_at_xy(x, 16 + game->n_player, "|   /       \\");
  print_at_xy(x, 17 + game->n_player, "|   \\       /");
  print_at_xy(x, 18 + game->n_player, "| 4  \\_____/  2");
  print_at_xy(x, 19 + game->n_player, "|");
  print_at_xy(x, 20 + game->n_player, "|       3");
  // unsigned int gap = game->n_player > 2 ? game->n_player : 2;
  gotoxy(0, MAX_MAP_SCREEN_HEIGHT + 2);
}

void print_game_result(Game *game) {
  assert(game_is_valid(game));
  clear();
  int *rank = malloc(sizeof(int) * game->n_player);
  verify(rank != NULL, "erreur d'allocation memoire");

  // giving players a rank
  for (int i = 0; i < game->n_player; i++) {
    rank[i] = i;
  }
  for (int j = 0, best_player = 0; j < game->n_player; j++) {
    for (int i = j; i < game->n_player; i++) {
      if (game->player_tab[rank[i]].score >
          game->player_tab[best_player].score) {
        best_player = i;
      }
    }
    rank[j] = best_player;
    rank[best_player] = j;
  }
  printf(CYN "Fin de la partie en %d tours!\n", game->n_turn);
  printf("Meilleur joueur : " BOLD_CYN "%s" CYN,
         game->player_tab[rank[0]].name);
  for (int i = 0; i < game->n_player; i++) {
    printf(BOLD_CYN "#%d " CYN ": %s (%d poissons)", i + 1,
           game->player_tab[rank[i]].name, game->player_tab[rank[i]].score);
  }
}

void print_game_map(Game *game) {
  gotoxy(0, 0);
  clear();
  for (int y = 0; y < N_ROWS; y++) {
    for (int x = 0; x < N_COLS; x++) {
      print_tile(x, y);
    }
  }
  // affichepp(&game);
  gotoxy(0, 0);
  print_game_infos(game);
}

/**
 * @brief Debug function to print the game data
 * @param game The game to print
 */
void debug_game_infos(Game *game) {

  printf("\nNombre de joueurs : %d\n", game->n_player);
  // printf("Nombre de tours : %d\n", game->n_turn);
  printf("Nombre de pingouins par joueurs : %d\n", game->n_peng_per_player);

  // players data
  printf("Liste des joueurs/joueuses : \n");
  for (unsigned int i = 0; i < game->n_player; i++) {
    printf("%s :\n", game->player_tab[i].name);
    printf("\tscore : %d\n", game->player_tab[i].score);

    printf("\tpenguins : \n");
    for (unsigned int j = 0; j < (game->n_peng_per_player); j++) {
      printf("\t\tn%d x = %d", j, game->player_tab[i].penguins[j].x);
      printf("  y = %d\n", game->player_tab[i].penguins[j].y);
    }
  }
  // TODO print debug map
}

void print_peng_test(Game *game) { // print_peng_test(&game);
  assert(game_is_valid(game));
  print_fishes(game->map);
  int screen_x = 0;
  int screen_y = 0;
  for (int i = 0; i < game->n_player; i++) {
    for (int j = 0; j < game->n_peng_per_player; j++) {
      screen_x = (game->player_tab[i].penguins[j].x) * (TILE_WIDTH - 2);
      screen_y = (game->player_tab[i].penguins[j].y) * (TILE_HEIGHT - 1);
      gotoxy(screen_x + 2, screen_y + 3);
      printf("🐧");
      // set bg to a color associated with the player
      printf("\x1B[%dmPi%d" RESET, i + 41, j + 1);
    }
  }
  gotoxy(0, MAX_MAP_SCREEN_HEIGHT + 2);
}

/**
 * @brief print the introduction screen
 *
 */
void print_intro() {
  printf(BOLD_CYN "Bienvenue sur CY-FISH 🐠!\n" RESET);
  printf(CYN "Regles : \n");
  printf("\t1. Chaque joueur deplace " BOLD_CYN
         "un pingouin en ligne droite" CYN " par tour.\n");
  printf(
      "\t2. Une fois qu'un pingouin 🐧 se déplace, il collecte les poissons qui "
      "etaient sur sa case de depart et la case en question est " BOLD_CYN
      "detruite ❌!\n" CYN);
  printf("\t3. La partie se termine quand " BOLD_CYN "plus aucun pingouin" CYN
         " ne peut se "
         "deplacer! \n");
  printf(
      "\t4. Le joueur qui a le plus de poissons gagne la partie !\n\n\n" RESET);
}

void print_game_start_text() {
  for (int i = 0; i < MAX_MAP_SCREEN_HEIGHT; i++) {
    fputc('\n', stdout);
  }
  print_img("Penguin.bmp");
  printf(BOLD_CYN "❄︎ Prêt ? ❄︎" CYN " (appuyez sur entrée)" RESET);
  while (getchar() != '\n') {
  }
  gotoxy(0, MAX_MAP_SCREEN_WIDTH + 1);
}

// GAME FUNCTIONS

/**
 * @brief Initialise penguin pos on tile with one fish of value one
 */
void init_penguins(Game *game) {
  assert(game_is_valid(game));

  // creating a random index tab to randomly select penguins
  int size = game->n_peng_per_player * game->n_player;
  int *index_tab = malloc(sizeof(int) * size);
  verify(index_tab != NULL, "erreur d'allocation memoire");

  for (int i = 0; i < size; i++) {
    index_tab[i] = i;
  }
  shuffle_tab(index_tab, size);

  // intialisation of penguins position
  int play_index = 0;
  int peng_index = 0;

  int i = 0;
  for (int y = 0; y < N_ROWS; y++) {
    for (int x = 0; x < N_COLS; x++) {

      assert(tile_correctly_init(game->map.tiles[y][x]));
      if (game->map.tiles[y][x].n_fishes == 1 &&
          game->map.tiles[y][x].fish_values[0] == 1) {
        play_index = index_tab[i] / game->n_peng_per_player;
        peng_index = index_tab[i] % game->n_peng_per_player;
        assert(penguin_is_valid(
            &(game->player_tab[play_index].penguins[peng_index])));

        game->map.tiles[y][x].penguin =
            &(game->player_tab[play_index].penguins[peng_index]);
        game->map.tiles[y][x].is_usable = false;
        game->player_tab[play_index].penguins[peng_index].x = x;
        game->player_tab[play_index].penguins[peng_index].y = y;
        i++;
      }
    }
  }

  assert(i >= size);
}

/**
 * @brief Initialise the game with player input
 * @param *game game to initialise
 */
Game game_new() {
  Game game;
  game.n_turn = 0;
  print_intro();
  game.n_player =
      val_between(N_MIN_PLAYERS, N_MAX_PLAYERS, "nombre de joueurs");
  assert(game.n_player >= N_MIN_PLAYERS && game.n_player <= N_MAX_PLAYERS);
  game.n_peng_per_player = 6 - game.n_player;
  assert(game.n_peng_per_player >= N_MIN_PENGUINS_PER_PLAYER &&
         game.n_peng_per_player <= N_MAX_PENGUINS_PER_PLAYER);

  game.player_tab = malloc(game.n_player * sizeof(Player));
  verify(game.player_tab != NULL, "erreur d'allocation memoire");
  init_player_tab(game.player_tab, game.n_player, game.n_peng_per_player);

  game.map = map_new();
  finish_starter_map(&game);
  init_penguins(&game);
  return game;
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
        if (game->map.tiles[y][x].fish_values[0] == 1) {
          nb_1_fish++;
        }
      }
    }
  }
  unsigned int nb_penguins = game->n_player * game->n_peng_per_player;
  int nx, ny;
  // make sure that at the beggining of the game, the map has enough case with
  // one fish of value 1
  while (nb_penguins > nb_1_fish) // dis is veri efectiv
  {
    nx = rand() % N_COLS;
    ny = rand() % N_ROWS;
    if (game->map.tiles[ny][nx].n_fishes != 1 ||
        game->map.tiles[ny][nx].fish_values[0] != 1) {
      game->map.tiles[ny][nx].n_fishes = 1;
      game->map.tiles[ny][nx].fish_values[0] = 1;
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
  assert(i_pin_to_move < game->n_peng_per_player);
  assert(game_is_valid(game));

  // update penguin pos
  int x = game->player_tab[play_turn].penguins[i_pin_to_move].x;
  int y = game->player_tab[play_turn].penguins[i_pin_to_move].y;
  game->player_tab[play_turn].penguins[i_pin_to_move].x = nx;
  game->player_tab[play_turn].penguins[i_pin_to_move].y = ny;
  assert(game->map.tiles[y][x].is_usable == false);

  // uptdate player score (variante FISH_GOLDEN) :
  for (int i = 0; i < game->map.tiles[y][x].n_fishes; i++) {
    game->player_tab[play_turn].score += game->map.tiles[y][x].fish_values[i];
  }

  // update map
  game->map.tiles[y][x].n_fishes = 0;
  game->map.tiles[y][x].penguin = NULL;
  game->map.tiles[ny][nx].is_usable = false;

  game->map.tiles[ny][nx].penguin =
      &(game->player_tab[play_turn].penguins[i_pin_to_move]);
}

// TEST FUNCTIONS

/**--
 * @brief Verify if the game is valid exit if not (redondant)
 * @param game to verify
 * @return true if the game is valid
 */
bool game_is_valid(Game *game) {
  // testing general game data
  if (game == NULL) {
    fprintf(stderr, "erreur : pointeur jeu nul\n");
    return false;
  }
  if (game->player_tab == NULL) {
    fprintf(stderr, "erreur : pointeur joueurs nul\n");
    return false;
  }
  if (game->n_player < N_MIN_PLAYERS || game->n_player > N_MAX_PLAYERS) {
    fprintf(stderr, "erreur : nombre de joueurs incorrect (%d)\n",
            game->n_player);
    return false;
  }
  if (game->n_peng_per_player < N_MIN_PENGUINS_PER_PLAYER ||
      game->n_peng_per_player > N_MAX_PENGUINS_PER_PLAYER) {
    fprintf(stderr, "erreur : nombre de pingouins par joueur incorrect\n");
    return false;
  }
  // testing players data
  for (int i = 0; i < game->n_player; i++) {
    if (!player_is_valid(&(game->player_tab[i]), game->n_peng_per_player)) {
      fprintf(stderr, "erreur : joueur %d incorrect", i);
      return false;
    }
  }
  // testing map data
  if (!map_is_valid(&(game->map))) {
    fprintf(stderr, "erreur : map invalide\n");
    return false;
  }
  return true;
}

bool tile_has_penguin(Game *game, unsigned int x, unsigned int y) {
  assert(game_is_valid(game));
  for (unsigned int i = 0; i < game->n_player; i++) {
    for (unsigned int j = 0; j < game->n_peng_per_player; j++) {
      if (game->player_tab[i].penguins[j].x == x &&
          game->player_tab[i].penguins[j].y == y) {
        return true;
      }
    }
  }
  return false;
}

bool can_player_play(Game *game, unsigned int play_turn) {
  assert(game_is_valid(game));
  assert(play_turn < game->n_player);
  for (unsigned int i = 0; i < game->n_peng_per_player; i++) {
    if (can_move_penguin(&game->map,
                         game->player_tab[play_turn].penguins + i)) {
      return true;
    }
  }
  return false;
}
