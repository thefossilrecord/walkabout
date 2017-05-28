#include <input.h>
#include <spectrum.h>


#include "levels.h"

/*

MEMORY MAP:

25000 - 25767 : Font
25768 - 25783 : Player Sprite
25784 - 25799 : Tiles

...

32768 -       : Code

*/

#define SCREEN_ATTRIBUTES 	22528
#define FONT 			25000
#define TILES 			25768

#define TILE_BYTES	32
#define TILE_PLAYER	TILES
#define TILE_TILE	TILE_PLAYER + TILE_BYTES
#define TILE_BLANK	TILE_TILE + TILE_BYTES
#define TILE_STAR	TILE_BLANK + TILE_BYTES

#define X_OFFSET	4
#define ATRR_LINE_WIDTH	32
#define KEYBOARD_WAIT	10
#define LIVES		3
#define START_LEVEL	2

#define STATE_PLAYING	1
#define STATE_DEAD	2
#define STATE_LEVELUP	3

struct in_UDK k;
void *joystick;
char px, py, state, lives, level, remaining;
char level_buffer[LEVEL_BUFFER_SIZE];
char game_over[]="GAME OVER!";
char rem_blocks[4]="000";
char life_count[4]="a:?";
//"111111211211100000100101104000000001101010101001101131313101100000001001100000001001100000001001210111111004110100000000010111111111121111111111";

//={0}; // 144 + 1

/*
128 If the character position is flashing, 0 if it is steady 
64 if the character position is bright, 0 if it is normal 
8* the code for the paper colour 
the code for the ink colour
*/

// black, blue, green, cyan, red, magenta
char block_attrs[MAX_TILES]={0 * 8 + 7, 1 * 8 +7, 4 * 8 + 7, 5 * 8 + 7, 2 * 8 + 7, 3 * 8 + 7};

// Takes a number and sticks in a string buffer.
void my_itoa(int value, char *buffer, int buffer_len)
	{
	char lookup[]= "0123456789";
	char i;

	// Fill string with leading zeros.
	for(i = 0; i < buffer_len; i++)
		buffer[i] = 0x30;
		
	i = buffer_len - 1;

	// Digits are added in in reverse order.
	while(i)
		{
		// Do modulo to pull out the least significant digit.
		int number = value % 10;
		// Tack this onto our buffer.
		buffer[i] = lookup[number];
		i--;
		// Shift number to the right.
		value = value / 10;
		}
	}

// Draws an attribute rectangle at x,y - w by h.	
void rect(char attr, char x, char y, char w, char h)
	{
	unsigned char *p;
	char i1 = 1, i2;
	for(; i1 <= h; i1++)
		{
		p = SCREEN_ATTRIBUTES + (y * ATRR_LINE_WIDTH) + x;
		i2 = 1;
		
		for(; i2 <= w; i2++)
			*p = attr;
			
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

	ld a,(hl)		// c = y	
	out (254),a
	
	#endasm
	}

/*
		ld	a, $7f
		in	a, (254)
		rra
		jr	nc, exit
		jr	main

		xor a
		in a, (254)
		bit 2,a
		
		bit 3,a
	
*/	
	
// Adapted assembly function from 'alexlotl' at
// https://www.rllmukforum.com/index.php?/topic/299583-adventures-in-zx-spectrum-dev/
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

// Draw a 16x16 tile.	
void draw_tile(char x, char y, unsigned char *tile)
	{
	unsigned char *p = addr_from_coords(x,y);
	draw_block2(p, tile);
	p = addr_from_coords(x,y+1);
	draw_block2(p,tile + 16);
	}
	
void draw_text(char x, char y, char *text)
	{
	unsigned char *p = addr_from_coords(x,y);	
	while(*text)
		{
		char offset = (*text) - 0x20;
		unsigned char *character = FONT + (offset * 8);

		draw_block(p, character);
		p++;
		text++;
		}
	}
	
void draw_player(char x, char y)
	{
	// Draw the player tile.
	unsigned char *tile = TILE_PLAYER;
	draw_tile((x * 2) + X_OFFSET, (y * 2), tile);
	}
	
void draw_level_tile(char x, char y, char b)
	{
	unsigned char *tile = TILE_TILE;
	unsigned char *p = SCREEN_ATTRIBUTES + ((y * 2) * 32) + (x * 2) + X_OFFSET;

	char attr = block_attrs[b - 48]; // Subtract ASCII 0 from block.
	// Fill in 2x2 attributes for tile colour.
	*p = attr;
	p++;
	*p = attr;
	p = p + 31;
	*p = attr;
	p++;
	*p = attr;	

	if(attr==7)
		// Drawing a blank.
		tile = TILE_STAR;//BLANK;

	draw_tile(X_OFFSET + (x * 2), y * 2, tile);
	}
	
void draw_level()
	{		
	int y = 0, x;
		
	char *block = level_buffer;

	for(; y < LEVEL_HEIGHT; y++)
		{
		x = 0;	
		for(; x < LEVEL_WIDTH; x++)
			{
			draw_level_tile(x, y, *block);
			block++;
			}
		}	
	
	//draw_text(8,20, level->name);
		
	draw_player(px, py);
	draw_text(0,23, rem_blocks);
	rect(2,0,21,1,1);
	life_count[2] = 0x30 + lives;
	draw_text(0,21, life_count);
	
	//draw_text(2,21, &life_count[2]);
	}
	
void move(char x, char y)
	{
	char new_x, new_y, prev;
//	unsigned char *tile;
	int offset;
		
	new_x = px + x;
	new_y = py + y;
	// Don't go out of bounds.
	if(new_x < 0 || new_x==LEVEL_WIDTH || new_y < 0 || new_y==LEVEL_HEIGHT)
		return;

	offset = (new_y * LEVEL_WIDTH) + new_x;
/*	
	*p = offset;
	p++;
	p++;
	*p = level_buffer[offset];
	
	#asm
	
brk:
	jr brk

	#endasm
*/	
	// Have we moved onto a blank tile?
	if(level_buffer[offset]==0x30)
		{
		state = STATE_DEAD;
		return;
		}
	
	// Decrement the tile we were on.
	offset = (py * LEVEL_WIDTH) + px;
	prev = level_buffer[offset];
	if(prev < 0x34)
		{
		// We can decrement this tile.
		prev--;
		level_buffer[offset] = prev;
		if(prev==0x30)
			{
			remaining--;
			my_itoa(remaining, rem_blocks, 3);
			draw_text(0,23, rem_blocks);
			if(!remaining)
				{
				state = STATE_LEVELUP;
				}
			}
		}
	// Erase our previous player position by redrawing the tile.
	draw_level_tile(px, py, prev);
	
	px = new_x;
	py = new_y;
	
	draw_player(px, py);
	}
	
void init_level(int level)
	{
	int copy = 0;
	struct level_data *current_level = &levels[level - 1];
	char *data = current_level->data;
	px = current_level->start_x;
	py = current_level->start_y;
	remaining = 0;

	// Copy level to our buffer.
	for(; copy < LEVEL_BUFFER_SIZE; copy++)
		{
		level_buffer[copy] = data[copy];
		switch(data[copy])
			{
			case 0x30:
			case 0x34:
				break;
			default:
				remaining++;
			}
		}
	
	remaining--;
	my_itoa(remaining, rem_blocks, 3);
	}

void init()
	{
	k.fire  = in_LookupKey('m');
	k.left  = in_LookupKey('o');
	k.right = in_LookupKey('p');
	k.up    = in_LookupKey('q');
	k.down  = in_LookupKey('a');
	
	// !!!		
	joystick = in_JoyKeyboard;
	lives = LIVES;
	level = START_LEVEL;
	}		
	
main()
	{
	char in_game = 1, wait = 0;
	unsigned char direction;
	
	cls(6);
		
	init();
		
	state = STATE_PLAYING;
	
	while(lives)
		{
		init_level(level);

		draw_level();
			
		while(state==STATE_PLAYING)
			{
			if(!wait)
				{
				direction = (joystick)(&k);
				if(direction & in_FIRE)
					state = STATE_DEAD;
				else if(direction==in_UP)
					{
					move(0, -1);
					wait = KEYBOARD_WAIT;
					}
				else if(direction==in_DOWN)
					{
					move(0, 1);
					wait = KEYBOARD_WAIT;
					}				
				else if(direction==in_LEFT)
					{
					move(-1, 0);
					wait = KEYBOARD_WAIT;
					}
				else if(direction==in_RIGHT)
					{
					move(1, 0);
					wait = KEYBOARD_WAIT;
					}
				}
			else
				{
				wait--;
#asm
				halt
#endasm		
				}
			}
		
		switch(state)
			{
			case STATE_DEAD:
				{
				// Lose a life!
				border(2);
				border(0);
				border(2);
				border(0);
					
				lives--;
				state = STATE_PLAYING;
				}
				break;
				
			case STATE_LEVELUP:
				{
				level++;
				state = STATE_PLAYING;	
				}
				break;
			}
		}

	cls(7);

	draw_text(10,10, game_over);
	}
	

