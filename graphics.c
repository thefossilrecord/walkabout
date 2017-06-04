
// General graphic routines / effects.

#include <spectrum.h>

#include "assets.h"
#include "graphics.h"

// Calculate an attribute value by adding together:
//
// 128 for FLASH
// 64 for BRIGHT
// 8 * PAPER
// INK

// Draws an attribute rectangle at x,y - w by h.	
void rect(char attr, char x, char y, char w, char h)
	{
	unsigned char *p;
	char i1 = 1, i2;
	for(; i1 <= h; i1++)
		{
		p = (unsigned char *)(SCREEN_ATTRIBUTES + (y * ATRR_LINE_WIDTH) + x);
		i2 = 1;
		
		for(; i2 <= w; i2++)
			{
			*p = attr;
			p++;
			}
			
		y = y + 1;
		}
	}

// Erases the screen and sets an initial attribute value.
void cls(int attr)
	{
	#asm
		
	ld hl,16384
	ld (hl),l
	ld de,16385
	ld bc,6143
	ldir
		
	ld hl,2
	add hl,sp		// skip over return address on stack	
	ld a,(hl)
	ld hl,SCREEN_ATTRIBUTES
	ld (hl),a
	ld de,SCREEN_ATTRIBUTES + 1
	ld bc,767
	ldir
		
	#endasm
	}
	
void border(char c)
	{
	#asm
		
	ld hl,2
	add hl,sp		// skip over return address on stack

	ld a,(hl)		// a = c	
	out (254),a
		
	// multiply border by 8 and store in BORDCR system variable
	// otherwise subsequent calls to BEEPER zaps the border colour.
	rlca
	rlca
	rlca
	ld (BORDCR), a	
	
	#endasm
	}

void fade()
	{
	#asm
	ld	a, 48
	ld	b, 7
fade:
	push 	bc
	halt
	halt
	ld	hl, SCREEN_ATTRIBUTES
	ld 	(hl), a
	ld 	de, SCREEN_ATTRIBUTES+1
	ld	bc, 767
	ldir
	pop	bc
	sub	8
	djnz	fade
		
	#endasm
	}

void draw_block(unsigned char *address, unsigned char *tile)
	{
/*		
	int index = 0;
	for(;index < 8; index ++)
		{
		*address = *tile;
		tile++;
		address = address + 256;
		}
*/		
	#asm
		
	ld hl,2
	add hl,sp		// skip over return address on stack

	ld e,(hl)		// put tile into de
	inc hl			 
	ld d,(hl)
	inc hl
	ld c,(hl)		// put address into bc
	inc hl
	ld b,(hl)
	ld h,b			// copy address into hl
	ld l,c
	ld b,8			// loop 8 times
		
copy:
	ld a,(de)
	ld (hl),a
	inc de
	inc h
	djnz copy

	#endasm
		
	}		
	
void draw_block2(unsigned char *address, unsigned char *tile)
	{
/*		
	int index = 0;
	for(;index < 8; index ++)
		{
		*address = *tile;
		tile++;
		address++;
		*address = *tile;
		address = address + 255;
		tile++;
		}		
*/		
	#asm
		
	ld hl,2
	add hl,sp		// skip over return address on stack

	ld e,(hl)		// put tile into de
	inc hl			 
	ld d,(hl)
	inc hl
	ld c,(hl)		// put address into bc
	inc hl
	ld b,(hl)
	ld h,b			// copy address into hl
	ld l,c
	ld b,8			// loop 8 times
		
copy2:
	ld a,(de)
	ld (hl),a
	inc de
	inc hl
	ld a,(de)
	ld (hl),a
	inc de
	dec hl
	inc h
	djnz copy2

	#endasm
	
	}

// Draw a 16x16 tile.	
void draw_tile(char x, char y, unsigned char *tile)
	{
	unsigned char *p = zx_cyx2saddr(y, x);
	draw_block2(p, tile);
	p = zx_cyx2saddr(y+1, x);
	draw_block2(p,tile + 16);
	}
	
void draw_text(char x, char y, char *text)
	{
	unsigned char *p = zx_cyx2saddr(y,x), *character;
	unsigned char v;
	while(*text)
		{
		v = *text;
/*			
		if(v > 128)
			{
			v = v - 0x80;
			character = (unsigned char *)(FONT + (v * 8));
			draw_block(p, character);
			break;
			}
*/
		v = v - 0x20;
		character = (unsigned char *)(FONT + (v * 8));

		draw_block(p, character);
		p++;
		text++;
		}
	}
