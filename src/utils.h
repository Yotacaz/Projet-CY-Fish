#ifndef SYSTEM_H
#define SYSTEM_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Text colors
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"

// background colors
#define BG_RED "\x1B[41m"
#define BG_GRN "\x1B[42m"
#define BG_YEL "\x1B[43m"
#define BG_BLU "\x1B[44m"
#define BG_MAG "\x1B[45m"
#define BG_CYN "\x1B[46m"
#define BG_WHT "\x1B[47m"

#define RESET "\x1B[0m"

#define clear() printf("\033[H\033[J")
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

char *scan_str(int max_len, char *msg);

int scan_int(char *msg); // int scanf with error handling

int val_between(int min, int max, char *msg);

void verify(int condition, char *err_msg);

#endif