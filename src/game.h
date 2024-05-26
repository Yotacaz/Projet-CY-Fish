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
  unsigned int n_player;
  unsigned int n_peng_per_player;
  unsigned int n_turn;
} Game;

bool game_is_valid(Game *game);

void print_game_infos(Game *game);

void debug_game_infos(Game *game);

void print_peng_test(Game *game);

void print_intro();
void print_game_start_text();

void print_game_result(Game *game);

void print_game_map(Game *game);

bool tile_has_penguin(Game *game, unsigned int x, unsigned int y);

Game game_new();

void init_penguins(Game *game);

void update_game_data(Game *game, unsigned int play_turn,
                      unsigned int i_pin_to_move, unsigned int nx,
                      unsigned int ny);

void finish_starter_map(Game *game);

bool can_player_play(Game *game, unsigned int play_turn);

#endif
