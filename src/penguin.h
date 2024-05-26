#ifndef _PENGUIN_H
#define _PENGUIN_H

#include <stdbool.h>

typedef struct Penguin {
  unsigned int x;
  unsigned int y;
} Penguin;

bool penguin_is_valid(Penguin *penguin);

#endif // _PENGUIN_H
