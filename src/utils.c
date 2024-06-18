#include "utils.h"
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Used to scan a string from the user with error handling / no overflow
 * @param max_len The maximum length of the string
 * @param *msg The message to display to the user
 * @return char* The string scanned
 */
char *scan_str(int max_len, char *msg) {
  assert(max_len > 1);
  assert(msg != NULL);
  char *buf = calloc((max_len + 3), sizeof(char));
  // we want an extra two char, one for reading an extra char from stdin to
  // ensure the texte isn't too long, the other for the \0
  verify(buf != NULL, "erreur d'allocation memoire");
  buf[max_len + 2] = '\0';

  bool too_long = 1; // to check if everything was sucessfully scaned

  do {
    printf("%s (maximum %d characteres) : ", msg, max_len);

    if (fgets(buf, max_len + 3, stdin) ==
        NULL) { // read max_len + 1 char + \n + \0
      free(buf);
      exit(EXIT_FAILURE);
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
      buf[len - 1] = '\0'; // Remove the newline character
      too_long = false;
    } else if (len == max_len + 2) {
      too_long = true; // Input is too long
      printf(RED "Erreur : nombre de characteres maximal atteint\n" RESET);
      int c;
      while ((c = getchar()) != '\n' && c != EOF) {
        // Clear the remaining characters in the input buffer
      }
    } else {
      too_long = false;
    }

  } while (too_long);

  char *str = malloc((strlen(buf) + 1) * sizeof(char));
  verify(str != NULL, "erreur d'allocation memoire");
  strcpy(str, buf); // Copy buffer to destination
  printf("name : %s\n", str);
  return str;
}

/**
 * @brief scans an POSITIVE int from the user with error handling
 * @param *msg The message to display to the user
 * @return int The int scanned
 */
int scan_int(char *msg) {
  int isOk;
  int n;
  do {
    printf("%s : ", msg);
    isOk = scanf("%d", &n);
    if (isOk != 1) {
      printf(BOLD_RED "echec du scanf, entrez un entier positif.\n" RESET);
    }
    if (n < 0) {
      printf(BOLD_RED "entrez une valeure positive\n" RESET);
      isOk = 0;
    }
    while (getchar() != '\n') {
    }
  } while (isOk != 1);
  return n;
}

/**
 * @brief asks the user to enter a int value between min and max
 * @param min The minimum value
 * @param max The maximum value
 * @param *msg The message to display to the user
 * @return int The int scanned
 */
int val_between(int min, int max, char *msg) {
  assert(min <= max);
  assert(msg != NULL);
  int isOk;
  int n;
  do {
    printf("%s (valeur entre %d et %d): ", msg, min, max);
    isOk = scanf("%d", &n);
    if (isOk != 1) {
      printf(BOLD_RED "echec du scanf\n" RESET);
    }
    if (n < min || n > max) {
      printf(BOLD_RED "entrez une valeure comprise entre %d et %d\n" RESET, min,
             max);
      isOk = 0;
    }
    while (getchar() != '\n') {
    }
  } while (isOk != 1);
  return n;
}

void print_at_xy(unsigned int x, unsigned int y, const char *format, ...) {
  assert(x < MAX_SCREEN_WID);
  gotoxy(x, y);
  // format code found on internet  gotoxy(x, y);
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

/**
 * @brief Verify for a coundition. Should be used instead of assert for error
 * that can't be handled but can happen independantly of the code
 * @param condition The condition to verify
 * @param err_msg The error message to display if the condition is not met
 */
void verify(int condition, char *err_msg) {
  if (condition == 0) {
    printf(BOLD_RED "%s\n" RESET, err_msg);
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief mix up the tab in param
 * @param tab the tab to shuffle
 * @param size the size of the tab
 */
void shuffle_tab(int *tab, size_t size) {
  assert(size > 0 && tab != NULL);
  int rand_index = 0;
  verify(tab != NULL, "erreur d'allocation memoire dans ini_penguin");

  for (int i = size - 1; i >= 0; i--) {
    rand_index = rand() % (i + 1);
    int tmp = tab[i];
    tab[i] = tab[rand_index];
    tab[rand_index] = tmp;
  }
}
