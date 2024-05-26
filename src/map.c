#include "map.h"
#include "game.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// PRINT FUNCTIONS
/**
 * @brief print an empty tile for the map
 */
void print_tile(int x, int y) {

  assert(coordinate_are_valid(x, y));
  int screen_x = x * (TILE_WIDTH - 2);
  int screen_y = y * (TILE_HEIGHT - 1);
  if (x % 2 == 0) {
    screen_y += 2;
  }
  gotoxy(screen_x + 2, screen_y);
  printf("_____");
  gotoxy(screen_x + 1, screen_y + 1);
  printf("/     \\");
  gotoxy(screen_x, screen_y + 2);
  printf("/       \\");
  gotoxy(screen_x, screen_y + 3);
  printf("\\       /");
  gotoxy(screen_x + 1, screen_y + 4);
  printf("\\_____/");
}

void print_fishes(Map map) {
  assert(map_is_valid(&map));
  for (int y = 0, screen_y = 0; y < N_ROWS; y++, screen_y += TILE_HEIGHT - 1) {
    for (int x = 0, screen_x = 0; x < N_COLS; x++, screen_x += TILE_WIDTH - 2) {
      screen_x = (x) * (TILE_WIDTH - 2);
      screen_y = (y) * (TILE_HEIGHT - 1);
      if (x % 2 == 0) {
        screen_y += 2;
      }
      gotoxy(screen_x + 2, screen_y + 2);

      if (map.tiles[y][x].n_fishes == 0) {
        printf("❌");
      } else {
        for (int i = 0; i < map.tiles[y][x].n_fishes; i++) {
          switch (map.tiles[y][x].fish_values[i]) {

          case 1:
            printf("🐟");
            break;
          case 2:
            printf("🐠");
            break;
          case 3:
            printf("🐡");
            break;
          default:
            assert(0);
          }
        }
      } // end of else
    }
  }
}

void map_debug(Map *map) {
  assert(map_is_valid(map));
  for (int y = 0; y < N_ROWS; y++) {
    printf(BOLD_MAG "y = %d\n" RESET, y);
    for (int x = 0; x < N_COLS; x++) {
      printf("x=%d n :%d, has_peng ? %d, utilisable ? %d - valeurs :", x,
             map->tiles[y][x].n_fishes, map->tiles[y][x].penguin != NULL,
             map->tiles[y][x].is_usable);
      for (int i = 0; i < map->tiles[y][x].n_fishes; i++) {
        printf(" %d", map->tiles[y][x].fish_values[i]);
      }
      putc('\n', stdout);
    }
    putc('\n', stdout);
  }
}

void debug_tile_with_peng(Map *map) {
  assert(map_is_valid(map));
  printf("Tiles with penguins :\n\n");
  for (int y = 0; y < N_ROWS; y++) {
    for (int x = 0; x < N_COLS; x++) {
      if (map->tiles[y][x].penguin != NULL) {
        printf("x=%d y=%d n :%d, utilisable ? %d\n", x, y,
               map->tiles[y][x].n_fishes, map->tiles[y][x].is_usable);
      }
    }
  }
}

// GAME FUNCTIONS
/**
 * @brief Initialise the game map with fishes data
 * @return the map
 */
Map map_new() {
  Map map;
  for (unsigned int y = 0; y < N_ROWS; y++) {
    for (unsigned int x = 0; x < N_COLS; x++) {
      map.tiles[y][x].penguin = NULL;
      map.tiles[y][x].is_usable = true;
      map.tiles[y][x].n_fishes = 1 + rand() % (MAX_FISHES);
      map.tiles[y][x].fish_values =
          malloc(sizeof(unsigned int) * map.tiles[y][x].n_fishes);
      verify(map.tiles[y][x].fish_values != NULL,
             "erreur d'allocation memoire");
      for (int k = 0; k < map.tiles[y][x].n_fishes; k++) {
        map.tiles[y][x].fish_values[k] = 1 + (rand() % 3);
      }
    }
  }
  return map;
}

// TEST FUNCTIONS

bool tile_is_valid(Tile *tile) {
  if (tile == NULL) {
    fprintf(stderr, "erreur : pointeur tile nul");
    return false;
  } else if (tile->fish_values == NULL) {
    fprintf(stderr, "pointeur fish_values nul\n");
    return false;
  } else if (tile->n_fishes > MAX_FISHES) {
    fprintf(stderr, "erreur : Nombre de poissons invalide (%d)\n",
            tile->n_fishes);
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
  } else if (tile.fish_values == NULL) {
    return false;
  }
  // fishes values
  for (int i = 0; i < tile.n_fishes; i++) {
    if (tile.fish_values[i] < 1 || tile.fish_values[i] > 3) {
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
        fprintf(stderr, "erreur : tile[%d][%d] invalide\n", y, x);
        return false;
      }
    }
  }
  return true;
}

/**
 * @brief Verify if coordinates are inferiors to the map size
 * @param x x coordinate
 * @param y y coordinate
 * @return true if the coordinates are valid
 */
bool coordinate_are_valid(unsigned int x, unsigned int y) {
  // kinda useless
  if (x >= N_COLS || y >= N_ROWS) {
    printf("coordonnes invalides x =%d y = %d\n", x, y);
    return false;
  }
  return true;
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
  /* return penguin->x % 2 == 0
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
                   map->tiles[penguin->x][penguin->y - 1].is_usable; */
  for (unsigned int i = 1; i < 7; i++) {
    if (can_move_penguin_to(map, penguin, i, 1)) {
      return true;
    }
  }
  return false;
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
    // printf("x=%u, y=%u, ", new_x, new_y);
    switch (direction) {
    case 1:
      if (new_x % 2 == 0) {
        new_x += 1;
      } else {
        new_x += 1;
        new_y -= 1;
      }
      break;
    case 2:
      if (new_x % 2 == 0) {
        new_x += 1;
        new_y += 1;
      } else {
        new_x += 1;
      }
      break;
    case 3:
      new_y += 1;
      break;
    case 4:
      if (new_x % 2 == 0) {
        new_x -= 1;
        new_y += 1;
      } else {
        new_x -= 1;
      }
      break;
    case 5:
      if (new_x % 2 == 0) {
        new_x -= 1;
      } else {
        new_y -= 1;
        new_x -= 1;
      }
      break;
    case 6:
      new_y -= 1;
      break;
    }
    if (new_x >= N_COLS || new_y >= N_ROWS) {
      return false;
    }
    // printf("nx=%u, ny=%u, ", new_x, new_y);

    Tile current_tile = map->tiles[new_y][new_x];
    // printf("usable=%d, has_penguin=%d\n", current_tile.is_usable,
    //        current_tile.penguin != NULL);
    if (!current_tile.is_usable || current_tile.penguin != NULL) {
      return false;
    }
  }

  return true;
}
