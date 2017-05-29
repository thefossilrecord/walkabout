#ifndef GFX_H
#define GFX_H

#define SCREEN_ATTRIBUTES 	22528
#define ATRR_LINE_WIDTH	32

#define BORDCR			23624

void rect(char attr, char x, char y, char w, char h);
void cls(int attr);
void border(char c);
void fade();
unsigned char * addr_from_coords(char x, char y);
void draw_block(unsigned char *address, unsigned char *tile);
void draw_block2(unsigned char *address, unsigned char *tile);

#endif