#include "game.h"
#include "map.h"
#include "penguin.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int affichage() { // J'ai repris ton code pour initialiser le jeu dans une
                  // fonction dans game.c
  int joueur;
  char prenom[20];
  char **tab;

  printf("quel est le nombre de joueur compris entre 2 et 6?\n");
  int A = scanf("%d", &joueur);
  if (A != 1) {
    exit(1);
  }
  if (joueur < 2 || joueur > 6) {
    printf("erreur, recommencez\n");
  }
  tab = (char **)malloc(joueur * sizeof(char *));
  if (tab == NULL) {
    exit(2);
  }

  for (int i = 0; i < joueur; i++) {
    printf("saisir les prenoms des joueurs\n");
    scanf("%s", prenom);

    tab[i] = (char *)malloc((strlen(prenom) * sizeof(char)));
    if (tab[i] == NULL) {
      exit(3);
    }
    strcpy(tab[i], prenom);
  }
  printf("les joueurs sont :\n");
  for (int i = 0; i < joueur; i++) {
    printf("%s\n", tab[i]);
  }
  free(tab);
}

int map(Game *game) {
  int n_cols_group = N_COLS / 2 + (N_COLS % 2 != 0);
  int x, y;
  clear();
  for (int i = 0; i < N_ROWS; i++) { // creation de la map
    for (int k = 0; k < n_cols_group; k++) {
      if (i == 0) {
        x = 1 + k * 16;
        y = 0;
        gotoxy(x, y);
        printf("           ______");
      }
    }

    for (int j = 0; j < n_cols_group; j++) {
      x = 2 + j * 16;
      y = 1 + i * 4; // 4 est le multiple des cases entre elles sur les colonnes
                     // si on met plus grand on aura des saut de ligne
      gotoxy(x, y);
      if (j == 0 && i == 1 + i * 16 || i == N_ROWS - 1) {
        printf("         /      ");
      } else {
        printf("  \\        /      ");
      }
    }

    for (int j = 0; j < n_cols_group; j++) {
      x = 3 + j * 16;
      y = 2 + i * 4;
      gotoxy(x, y);
      if (j == 0 && i == 0) {
        printf("   ______/       ");
      } else {
        printf("  \\______/       ");
      }
    }

    for (int j = 0; j < n_cols_group; j++) {
      x = 4 + j * 16; // on ne change plus x
      y = 3 + i * 4;  // y rajoute 1 a chaque boucle
      gotoxy(x, y);
      printf(" /      \\       ");
    }
    for (int j = 0; j < n_cols_group; j++) {
      x = 4 + j * 16;
      y = 4 + i * 4;
      gotoxy(x, y);
      if (i == N_ROWS - 1) {
        printf("/        \\");
      } else {
        printf("/        \\______");
      }
    }

    // nombre de joueur
    // nb de pingouins par joueur
    // position x du j penguin
    // position y du i joueur
    for (int i = 0; i < game->n_player; i++) {
      for (int j = 0; j < game->n_ping_per_player; j++) {
        int px = 3 + game->player_tab[i].penguins[j].x * 16;
        int py = 2 + game->player_tab[i].penguins[j].y * 4;
        gotoxy(x, y);
        if (game->map
                .tiles[game->player_tab[i].penguins[j].y]
                      [game->player_tab[i].penguins[j].x]
                .n_fishes > 0) {
          printf("🐧");
        } else {
          printf("❌");
        }
      }
    }
  }
}

int main() {
  // todo list : init pos pinguin, function to get player input, main game loop,
  // color code, finish game_display
  _Bool exit = 0;
  Game game;
  char c[] = "\U0001F427";
  printf("%s", c);
  // int tab = affichage();
  init_game(&game);
  print_game_infos(&game); // for debug only
  // init_map_string(game.map_str);
  //  print_map(&game, game.map_str);
  //   int mapjeux = map(&game);
  //   printf(" %d", mapjeux);
  //    INITIALISATION

  do { // Main game loop
       // get player input
       // update game data
       // display game
    exit = 1;
  } while (exit == 0);

  return 0;
}