
// General graphic routines / effects.

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

// Adapted assembly function from 'alexlotl' at
// https://www.rllmukforum.com/index.php?/topic/299583-adventures-in-zx-spectrum-dev/
// Takes x (0 - 31) and y (0 - 23) and converts to a screen address.
// Co-ordinates loaded into bc
// Co-ordinate screen address returned in hl
unsigned char * addr_from_coords(char x, char y)
	{
	#asm
		
	ld hl,2
	add hl,sp		// skip over return address on stack

	ld c,(hl)		// c = y
	inc hl			// need to skip 2 bytes
	inc hl
	ld b,(hl)		// b = x
		
	ld a,c 			// copy y-coord to accumulator
	and %00011000		// Wipes out all but the bits relating to band
	or %01000000		// adds the 010 prefix
	ld h,a			// copy to MSB of destination
	ld a,c			// re-copy y-coord to accumulator
	and %00000111		// Wipes out all but the bits relating to row within band
	rra			// rotate right 4 times: 0000 00YY (Y)
	rra			// Y000 000Y (Y)
	rra			// YY00 0000 (Y)
	rra			// YYY0 0000 (0)
	add a,b			// simply add x-coord
	ld l,a			// hl now contains address

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
	dec b
	jr nz,copy

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
	dec b
	jr nz,copy2

	#endasm
	
	}
