#include "bmp_img.h"
#include "game.h"
#include "map.h"
#include "penguin.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void affichepp(Game *game) {
  Tile *valuesfishes;

  for (int y = 0; y < N_ROWS; y++) {
    for (int x = 0; x < N_COLS; x++) {
      int screen_x = x * (TILE_WIDTH - 2);
      int screen_y = y * (TILE_HEIGHT - 1);
      if (x % 2 == 0) {
        screen_y += 2;
      }
      gotoxy(screen_x, screen_y);
      /*int nfishes = game->map.tiles[y][x].n_fishes;
      // affichage des poissons dans les cases
      if (nfishes == 1) {
        gotoxy(screen_x + 1, screen_y + 2);
        printf("🐟");
      } else if (nfishes == 2) {
        gotoxy(screen_x + 2, screen_y + 2);
        printf("🐠");
      } else if (nfishes == 3) {
        gotoxy(screen_x + 1, screen_y + 3);
        printf("🐡");
       } else
         printf("?")
        ;
       }*/
      for (int i = 0; i < game->map.tiles[y][x].n_fishes; i++) {
        int fishes = valuesfishes->fish_values[3];
        // affichage des poissons dans les case
        if (fishes == 1) {
          gotoxy(screen_x + 1, screen_y + 2);
          printf("🐟");
        } else if (fishes == 2) {
          gotoxy(screen_x + 2, screen_y + 2);
          printf("🐠");

        } else if (fishes == 3) {
          gotoxy(screen_x + 1, screen_y + 3);

          printf("🐡");
        } else {
          printf("?");
        }
        // affichage des pingouins si il y a au moins un poisson sur la cas
        if (fishes > 0) {
          gotoxy(screen_x + 2, screen_y + 1);
          printf("🐧");
        } else if (fishes == 0)
          gotoxy(screen_x + 2, screen_y + 1);
        printf("❌"); // affichage des croix si la case est cassé
      }
    }
  }
}

int main() {
  srand(time(NULL));
  assert(N_COLS >= 4);
  assert(N_ROWS >= 3);
  // INITIALISATION
  bool exit = 0;
  Game game;
  game = game_new();
  clear();
  gotoxy(0, 0);
  print_game_start_text();

  // MAIN LOOP
  while (can_game_continue(&game)) {
    unsigned current_player = game.n_turn % game.n_player;

    if (can_player_play(&game, current_player)) {

      clear();
      // DISPLAY GAME
      print_game_map(&game);
      print_peng_test(&game);

      unsigned int dir, dist, ipen; // direction, distance, id penguin
      Penguin *pen;

      printf("Au tour de \x1B[%dm%s" RESET " :\n", current_player + 41,
             game.player_tab[current_player].name);

      // PLAYER INPUT
      bool first = true;
      do {
        if (!first) {
          printf(RED "Déplacement impossible\n" RESET);
        }
        ipen = val_between(1, game.n_peng_per_player, "Pingouin numero ") - 1;
        pen = game.player_tab[current_player].penguins + ipen;
        dir = val_between(1, 6, "Direction de deplacement");
        dist = scan_int("Distance de deplacement (debug : 666)");
        first = false;
        if (dist == 666) {
          debug_game_infos(&game);
          map_debug(&(game.map));
        }
      } while (!can_move_penguin_to(&(game.map), pen, dir, dist));

      // printf("dist=%d, ", dist);

      // CALCULATING RESULTING POSITION
      unsigned int nx, ny;
      switch (dir) {
      case 1:
        nx = pen->x + dist;
        ny = pen->y - (dist + pen->x % 2) / 2;
        break;
      case 2:
        nx = pen->x + dist;
        ny = pen->y + (dist + !(pen->x % 2)) / 2;
        break;
      case 3:
        nx = pen->x;
        ny = pen->y + dist;
        break;
      case 4:
        nx = pen->x - dist;
        ny = pen->y + (dist + !(pen->x % 2)) / 2;
        break;
      case 5:
        nx = pen->x - dist;
        ny = pen->y - (dist + pen->x % 2) / 2;
        break;
      case 6:
        ny = pen->y - dist;
        nx = pen->x;
        break;
      }
      update_game_data(&game, current_player, ipen, nx, ny);

    } else {
      print_game_map(&game);
      print_peng_test(&game);
      gotoxy(0, MAX_MAP_SCREEN_HEIGHT + 5);

      printf("Tour de \x1B[%dm%s" RESET " passe, il/elle ne peut plus jouer\n",
             current_player + 41, game.player_tab[current_player].name);
      printf("Appuyez sur entree pour continuer");
      while (getchar() != '\n') {
      }
    }
    game.n_turn++;
  }
  print_game_map(&game);
  print_peng_test(&game);
  gotoxy(0, MAX_MAP_SCREEN_HEIGHT + 5);
  print_game_result(&game);
  return 0;
}
