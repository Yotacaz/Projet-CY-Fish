#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "penguin.h"
#include "player.h"
#include "utils.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// VARIANTES START_AUTO & FISH_GOLDEN

typedef struct _game {
  Player *player_tab;
  Map map;
  Map_elem map_str[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH]; // map_str[y][x]
  unsigned int n_player;
  unsigned int n_ping_per_player;
} Game;

bool game_is_valid(Game *game);

void print_game_infos(Game *game);

void print_map(Game *game, char map_str[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH]);

bool tile_has_penguin(Game *game, unsigned int x, unsigned int y);

void init_game(Game *game);

void update_game_data(Game *game, unsigned int play_turn,
                      unsigned int i_pin_to_move, unsigned int nx,
                      unsigned int ny);

void finish_starter_map(Game *game);

#endif