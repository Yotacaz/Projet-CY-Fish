#ifndef _MAP_H
#define _MAP_H

#include "penguin.h"
#include <stdbool.h>
#include <stdio.h>

#define N_ROWS 5
#define N_COLS 9
#define MAX_FISHES 3

#define TILE_WIDTH 9
// tile logical width is 9 - 2 = 7 as 2 are for shared border, real width is 9
#define TILE_HEIGHT 5
// tile logical height is 5 - 1 = 4 as 1 are for shared border, real height is 5

#define MAX_MAP_SCREEN_WIDTH ((TILE_WIDTH - 2) * N_COLS) + 2
//+2 is for 2 final char of hexagon
#define MAX_MAP_SCREEN_HEIGHT ((TILE_HEIGHT - 1) * N_ROWS) + 2
//+2 is for height variation of hexagon

typedef struct {
  unsigned int n_fishes;
  int *fish_values; // tab where values of fishes are stored for variant
                    // FISH_GOLDEN
  Penguin *penguin;
  bool is_usable;
} Tile;

typedef struct {
  Tile tiles[N_ROWS][N_COLS]; //[y][x]
} Map;

bool tile_is_valid(Tile *tile);
bool tile_correctly_init(Tile tile);
bool map_is_valid(Map *map);
bool coordinate_are_valid(unsigned int x, unsigned int y);

Map map_new();

void map_debug(Map *map);

void debug_tile_with_peng(Map *map);

void print_tile(int x, int y);

void print_fishes(Map map);

bool is_neighbor(unsigned int x1, unsigned int y1, unsigned int x2,
                 unsigned int y2);

bool can_move_penguin(Map *map, Penguin *penguin);

bool can_move_penguin_to(Map *map, Penguin *penguin, unsigned int direction,
                         unsigned int distance);

#endif // _MAP_H
