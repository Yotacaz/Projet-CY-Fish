#ifndef _BMP_IMG_H
#define _BMP_IMG_H

#define IM_MAX_SIZE 400 * 400 // max size of img
#define BIT_PER_PIXEL 3

#define PIX_SIZE 2 // in spacebar
#define CMD_LEN 10 // defined by "x1B[%d;%d;%dm"
#define RES_LEN 4  // defined by \x1B[0m

typedef struct im {
  int wid, hei;
  unsigned char *pixel;
} Image;

int get_little_endian(unsigned char *buf);

int get_2_bytes_little_endian(unsigned char *buf);

void check_header(unsigned char *buf);

Image im_to_rvb(char file_name[]);

char *create_str(Image img);

void print_img(char *path);

#endif
