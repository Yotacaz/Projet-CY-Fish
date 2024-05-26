#include "bmp_img.h"
#include "utils.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// use https://en.wikipedia.org/wiki/BMP_file_format for reference
// or https://www.youtube.com/watch?v=kpHFFFu9qeU for short

int get_little_endian(unsigned char *buf) {
  /*everything in a BMP file is little endian, this mean that the first byte is
  the least significant and the last byte the most significant, original
  get_big_endian() for png found on internet
  */
  return ((unsigned char)buf[3] << 24) | ((unsigned char)buf[2] << 16) |
         ((unsigned char)buf[1] << 8) | (unsigned char)buf[0];
}

int get_2_bytes_little_endian(unsigned char *buf) {
  return ((unsigned char)buf[1] << 8) | (unsigned char)buf[0];
}

void check_header(unsigned char *buf) {
  // Check that the file type(BMP) is correct
  verify(buf[0] == 'B' && buf[1] == 'M', "IMG : file must be of type bmp");
  verify(get_2_bytes_little_endian(buf + 28) == 24,
         "IMG : only 3 byte per pixel is supported");
  verify(get_little_endian(buf + 30) == 0,
         "IMG : no compression method supported");
  // skipped "number of color planes (must be 1)", idk what it is
}

Image im_to_rvb(char file_name[]) {
  // verifs
  assert(file_name != NULL);
  Image img;
  unsigned char *buf = malloc(IM_MAX_SIZE * sizeof(char));
  if (buf == NULL) {
    printf("Erreur d'allocation memoire \n");
    exit(EXIT_FAILURE);
  }
  FILE *fichier = fopen(file_name, "rb");
  if (fichier == NULL) {
    printf("Erreur : le fichier '%s' n'a pas pu etre ouvert\n", file_name);
    exit(EXIT_FAILURE);
  }
  unsigned int size = fread(buf, 1, IM_MAX_SIZE, fichier);

  // general header infos :
  check_header(buf);
  verify(
      size == ((unsigned int)get_little_endian(buf + 2)),
      "Taille de fichier corrompue ou taille maximale atteinte"); // len = size
  int offset =
      get_little_endian(buf + 10); // offset where actual image data starts
  // width and height of image :
  img.wid = get_little_endian(buf + 18);
  img.hei = get_little_endian(buf + 22);

  // initialising usefull variables and malloc
  int p_size = img.wid * img.hei;

  img.pixel = malloc(sizeof(unsigned char) *
                     p_size); // for 3 bits colors values, 1 char = 1 pixel

  verify(img.pixel != NULL, "Erreur d'allocation memoire pour image");

  // storing each pixel data in img.pixel

  int start_pos = p_size - img.wid; // pixel are stored from bottom to top
  // each row of pixel must contain a multipe of 4 bytes, there is therefore a
  // padding at the end of each row that we don't need to account for
  int padding = (4 - (img.wid * 3) % 4) * (img.wid % 4 != 0);
  int pos = offset; // position in buffer

  // For 3 bit color
  char r, g, b;
  for (int y = 0; y < p_size; y += img.wid) {
    for (int x = 0; x < img.wid; x++) {

      /*in bmp, pixels are sorted from botom to top and RGB values are actualy
      BGR we will keep it like that because to print for exemple red (FF0000
      in hex) is the second color in the ANSI 3bit table, same goes for green
      ...*/
      r = (buf[pos] > 127); // we determine if our red value is closer to full
                            // red or no red
      g = (buf[pos + 1] > 127);
      b = (buf[pos + 2] > 127);
      img.pixel[start_pos + x - y] =
          (r << 2) | (g << 1) | (b); // y axis is reverted (for use ease)

      // for rgb : pixel[p_size - (x+y*wid)] = (buf[pos]<<16) | (buf[pos +
      // 1])<<8 |(buf[pos + 2]);    //RGB
      pos += 3;
    }
    pos += padding;
  }

  fclose(fichier);
  // free(fichier);
  free(buf);
  return img;
}

char *create_str(Image img) {
  /*if array = 3 blue pixels + 1 red (+new line) + 3 green
  ==> create_str should return : "BLUE    RED  \nGREEN      \0" */

  int len = 1; // contain a least '\0'

  char current;
  for (int y = 0; y < img.wid * img.hei;
       y += img.wid) { // getting result string length
    for (int x = 0; x < img.wid; x++) {
      len += CMD_LEN + PIX_SIZE;
      current = img.pixel[x + y];
      for (int i = x + y + 1; i < img.wid + y && current == img.pixel[i];
           i++) { // check for similar pixel color
        len += PIX_SIZE;
        x++;
      }
    }
    len +=
        RES_LEN + 1; // we reset the color at the end of each line and add '\n'
  }
  verify(len >= (1 + CMD_LEN + PIX_SIZE + RES_LEN + 2),
         "Une image 1x1 pixel devrait etre au moins 15 char longue");

  char *img_str = calloc(len, sizeof(char)); // init at 0 for testing

  verify(img_str != NULL, "Erreur d'allocation memoire");

  int k = 0;
  for (int y = 0; y < img.wid * img.hei;
       y += img.wid) { // setting the string to its final value.
    for (int x = 0; x < img.wid; x++) {

      sprintf(img_str + k, "%c[%d;%d;%dm", 0x1B, 0, 0 + 30,
              img.pixel[x + y] +
                  40); // idk how that work, found on
                       // https://www.linuxjournal.com/article/8603
      k += CMD_LEN;
      img_str[k] = ' ';
      img_str[k + 1] = ' ';
      k += PIX_SIZE;
      current = img.pixel[x + y];
      for (int i = x + y + 1; i < img.wid + y && current == img.pixel[i]; i++) {
        img_str[k] = ' ';
        img_str[k + 1] = ' ';
        k += PIX_SIZE;
        x++;
      }
    }
    sprintf(img_str + k, "\x1B[0m"); // add reset in str \x1B[0m
    k += RES_LEN;
    img_str[k] = '\n';
    k += 1;
  }
  img_str[len - 1] = '\0';

  return img_str;
}

void print_img(char *path) {
  assert(path != NULL);
  Image img = im_to_rvb(path);

  char *img_str = create_str(img);
  puts(img_str);
  free(img_str);
  free(img.pixel);
}
