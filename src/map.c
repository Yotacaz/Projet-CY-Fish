#include "map.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool tile_is_valid(Tile *tile) {
  if (tile == NULL) {
    return false;
  } else if (tile->fishValues == NULL) {
    printf("pointeur fishValues nul\n");
    return false;
  } else if (tile->n_fishes > MAX_FISHES) {
    printf("n_fishes = %d\n", tile->n_fishes);
    return false;
  }

  return true;
}

/**
 * @brief Debug function to verify if a tile is corectly initialised
 * @param tile to verify
 * @return true if the game is valid
 */
bool tile_correctly_init(Tile tile) {
  // general tile data
  if (tile.n_fishes <= 0 || tile.n_fishes > MAX_FISHES) {
    return false;
  } else if (tile.is_usable != 1) {
    return false;
  } else if (tile.fishValues == NULL) {
    return false;
  }
  // fishes values
  for (int i = 0; i < tile.n_fishes; i++) {
    if (tile.fishValues[i] < 1 || tile.fishValues[i] > 3) {
      printf("valeur du poisson doit etre 1, 2 ou 3 en mode de comptage "
             "poisson dore");
      return false;
    }
  }
  return true;
}

bool map_is_valid(Map *map) {
  if (map == NULL) {
    return false;
  }
  for (int y = 0; y < N_ROWS; y++) {
    for (int x = 0; x < N_COLS; x++) {
      if (!tile_is_valid(&(map->tiles[y][x]))) {
        printf("tile[%d][%d] invalide\n", y, x);
        return false;
      }
    }
  }
  return true;
}

bool coordinate_are_valid(unsigned int x, unsigned int y) {
  // kinda useless
  return x < N_COLS && y < N_ROWS;
}

void init_map_string(Map_elem map_str[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH]) {
  assert(map_str != NULL);
  for (int screen_y = 0; screen_y < MAX_SCREEN_HEIGHT; screen_y++) {
    for (int screen_x = 0; screen_x < MAX_SCREEN_WIDTH; screen_x++) {
      map_str[screen_y][screen_x].bg_color = res;
      *(map_str[screen_y][screen_y].elem) = ' ';
      map_str[screen_y][screen_x].len = 1;
    }
  }
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
  for (int screen_y = 0; screen_y < MAX_SCREEN_HEIGHT; screen_y++) {
    map_str[screen_y][MAX_SCREEN_WIDTH - 1] = '\0';
  }
}

void print_tile(Map_elem map_str[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH],
                unsigned int scn_y, unsigned int scn_x) {

  assert(scn_y >= 0 && scn_y < MAX_SCREEN_HEIGHT - 4);
  assert(scn_x >= 0 && scn_x < MAX_SCREEN_WIDTH - 9);
  // begining of tile has a 2 pixel offset compared to its left ...
  for (int i = 0; i < 5; i++) {
    map_str[scn_y][scn_x + 2 + i].type = underscore; //_____
  }
  map_str[scn_y + 1][scn_x + 1].type = slash; //: /     \\
	map_str[scn_y + 1][scn_x + 7].type = backslash;
	
  map_str[scn_y + 2][scn_x].type = slash;	//: /       \\
  map_str[scn_y + 2][scn_x + 8].type = backslash;
	
	  map_str[scn_y + 3][scn_x].type = backslash;	//: \\       /
	  map_str[scn_y + 3][scn_x + 8].type = slash;
	
	  map_str[scn_y + 4][scn_x + 1].type = backslash;	//: \\_____/
	for (int i = 0; i < 5; i++) {
		map_str[scn_y+4][scn_x + 2 + i].type = underscore; //_____
	  }

	memcpy(map_str[scn_y + 3] + scn_x, "", 9);
  memcpy(map_str[scn_y + 4] + scn_x + 1, "", 7);
}

Map map_new() {
  Map map;
  for (unsigned int y = 0; y < N_ROWS; y++) {
    printf("y = %d", y);
    for (unsigned int x = 0; x < N_COLS; x++) {

      map.tiles[y][x].is_usable = true;
      map.tiles[y][x].n_fishes = 1 + rand() % (MAX_FISHES);
      map.tiles[y][x].fishValues =
          malloc(sizeof(unsigned int) * map.tiles[y][x].n_fishes);
      verify(map.tiles[y][x].fishValues != NULL, "erreur d'allocation memoire");
      for (int k = 0; k < map.tiles[y][x].n_fishes; k++) {
        map.tiles[y][x].fishValues[k] = 1 + (rand() % 3);
      }
    }
  }
  return map;
}

bool is_neighbor(unsigned int x1, unsigned int y1, unsigned int x2,
                 unsigned int y2) {
  return (x1 == x2 && y1 == y2) || (x1 == x2 && y1 - 1 == y2) ||
         (x1 == x2 && y1 + 1 == y2) || (x1 - 1 == x2 && y1 == y2) ||
         (x1 + 1 == x2 && y1 == y2) ||
         (x1 % 2 == 0 && x1 - 1 == x2 && y1 - 1 == y2) ||
         (x1 % 2 == 0 && x1 + 1 == x2 && y1 - 1 == y2) ||
         (x1 % 2 == 1 && x1 - 1 == x2 && y1 + 1 == y2) ||
         (x1 % 2 == 1 && x1 + 1 == x2 && y1 + 1 == y2);
}

bool can_move_penguin(Map *map, Penguin *penguin) {
  assert(map_is_valid(map));
  assert(penguin_is_valid(penguin));
  return penguin->x % 2 == 0
             ? map->tiles[penguin->x + 1][penguin->y].is_usable ||
                   map->tiles[penguin->x + 1][penguin->y + 1].is_usable ||
                   map->tiles[penguin->x][penguin->y + 1].is_usable ||
                   map->tiles[penguin->x - 1][penguin->y + 1].is_usable ||
                   map->tiles[penguin->x - 1][penguin->y].is_usable ||
                   map->tiles[penguin->x][penguin->y - 1].is_usable
             : map->tiles[penguin->x + 1][penguin->y - 1].is_usable ||
                   map->tiles[penguin->x + 1][penguin->y].is_usable ||
                   map->tiles[penguin->x][penguin->y + 1].is_usable ||
                   map->tiles[penguin->x - 1][penguin->y].is_usable ||
                   map->tiles[penguin->x - 1][penguin->y - 1].is_usable ||
                   map->tiles[penguin->x][penguin->y - 1].is_usable;
}

/**
 * @brief Check if the penguin can move ine the specified direction
 * @param map The map
 * @param penguin The penguin to move
 * @param direction The direction to move (1-6 clockwise)
 * @param distance The distance to move
 */
bool can_move_penguin_to(Map *map, Penguin *penguin, unsigned int direction,
                         unsigned int distance) {
  assert(map_is_valid(map));
  assert(penguin_is_valid(penguin));
  assert(direction >= 1 && direction <= 6);
  assert(distance > 0);

  unsigned int new_x = penguin->x;
  unsigned int new_y = penguin->y;

  for (unsigned int i = 0; i < distance; i++) {
    if (new_x >= N_COLS || new_y >= N_ROWS) {
      return false;
    }

    Tile current_tile = map->tiles[new_x][new_y];
    if (!current_tile.is_usable || current_tile.penguin != NULL) {
      return false;
    }

    switch (direction) {
    case 1:
      if (new_x % 2 == 0) {
        new_x += 1;
      } else {
        new_x += 1;
        new_y -= 1;
      }
    case 2:
      if (new_x % 2 == 0) {
        new_x += 1;
        new_y += 1;
      } else {
        new_x += 1;
      }
    case 3:
      new_y += 1;
    case 4:
      if (new_x % 2 == 0) {
        new_x -= 1;
        new_y += 1;
      } else {
        new_x -= 1;
      }
    case 5:
      if (new_x % 2 == 0) {
        new_x -= 1;
      } else {
        new_y -= 1;
        new_x -= 1;
      }
    case 6:
      new_y -= 1;
    }
  }

  return true;
}