#include "penguin.h"

#include "map.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

bool penguin_is_valid(Penguin *penguin) {
  return penguin != NULL && penguin->x < N_COLS && penguin->y < N_ROWS;
}
