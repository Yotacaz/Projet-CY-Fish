#ifndef _MAP_H
#define _MAP_H

#include "penguin.h"
#include <stdbool.h>
#include <stdio.h>

#define N_ROWS 5
#define N_COLS 8
#define MAX_FISHES 3

#define TILE_WIDTH 7
// tile logical width is 9 - 2 = 7 as 2 are for shared border, real width is 9
#define TILE_HEIGHT 4
// tile logical height is 5 - 1 = 4 as 1 are for shared border, real height is 5
#define MAX_SCREEN_WIDTH (TILE_WIDTH * N_COLS + 3)
//+2 for 2 final char of hexagon +1 for \0
#define MAX_SCREEN_HEIGHT (TILE_HEIGHT * N_ROWS + 3)
//+2 for height variation of hexagon +1 for \0

typedef enum { red, grn, blu, mag, cyn, wh, res } Bg_colors;
typedef enum {
  peng,
  fish1,
  fish2,
  fish3,
  underscore,
  slash,
  backslash
} Char_type;

typedef struct {
  unsigned int n_fishes;
  int *fishValues; // tab where values of fishes are stored for variant
                   // FISH_GOLDEN or FISH_ROTTEN
  Penguin *penguin;
  bool is_usable;
} Tile;

typedef struct {
  Char_type type;
  unsigned int len;
  Bg_colors bg_color;
} Map_elem;

typedef struct {
  Tile tiles[N_ROWS][N_COLS]; //[y][x]
} Map;

void init_map_string(Map_elem map[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH]);

void print_tile(Map_elem map_str[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH],
                unsigned int scn_y, unsigned int scn_x);

bool tile_is_valid(Tile *tile);
bool tile_correctly_init(Tile tile);
bool map_is_valid(Map *map);
bool coordinate_are_valid(unsigned int x, unsigned int y);

Map map_new();

bool is_neighbor(unsigned int x1, unsigned int y1, unsigned int x2,
                 unsigned int y2);

bool can_move_penguin(Map *map, Penguin *penguin);

bool can_move_penguin_to(Map *map, Penguin *penguin, unsigned int direction,
                         unsigned int distance);

#endif // _MAP_H
