/******************************************************************************
 *
 * Copyright(c) 2017 Bob Fossil. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/

#include <input.h>
#include <spectrum.h>
#include <im2.h>

#include "assets.h"
#include "graphics.h"
#include "levels.h"
#include "main.h"
#include "menu.h"
#include "sound.h"

/*

TO DO:

- Effects on menu?
- Replace menu asm input with C keyboard?

MEMORY MAP:

25000 - 25767 : Font

25768 - 25799 : Player Sprite
25800 - 25831 : Player Sprite Frame 2
25832 - 25863 : Player Sprite Rotated 90 degress
25864 - 25895 : Player Sprite Rotated 180 degress
25896 - 25927 : Player Sprite Rotated 270 degress
25928 - 25959 : Level Tile
25960 - 26279 : Background Tiles 1 - 10

32768 -       : Code
50176 - 50432 : IM2 Table
50433	      : Previous I value
50629 - 50631 : Interrupt jump instruction
*/


#define INTERRUPT_TABLE		50176	// $c400
#define INTERRUPT_JUMP		50629	// $c5c5
#define INTERRUPT_STORE		50433

#define X_OFFSET	4
#define KEYBOARD_WAIT	10
#define LIVES		3
#define START_LEVEL	1

#define CODE_X_POS	11
#define CODE_Y_POS	12



struct in_UDK k;
void *joystick;
char px, py, state, lives, level, remaining, backg, backg_attr;
char level_buffer[LEVEL_BUFFER_SIZE];
char game_over[]="GAME OVER!";
char rem_blocks[4]="000";
char life_count[4]="^:?";
char level_txt[4]="LVL";
char level_count[4]="000";
char lookup[]= "0123456789";
int interrupt_timer = 0;

uchar in_KeyDebounce = 1;	// no debouncing
uchar in_KeyStartRepeat = 0;	// wait 20/50s before key starts repeating
uchar in_KeyRepeatPeriod = 0;	// 10;  // repeat every 10/50s
uint in_KbdState;               // reserved

unsigned char *backgrounds[MAX_BACKGROUND]=
	{
	TILE_BACKGROUND_BLANK,
	TILE_BACKGROUND_SQUARES,
	TILE_BACKGROUND_STAR,
	TILE_BACKGROUND_MESH,
	TILE_BACKGROUND_TOYBLOCK,
	TILE_BACKGROUND_STRIPES,
	TILE_BACKGROUND_DOTS,
	TILE_BACKGROUND_DIAMOND,
	TILE_BACKGROUND_GRID,
	TILE_BACKGROUND_NET
	};

char block_attrs[MAX_TILES]=
	{
	PAPER_BLACK + INK_WHITE,
	PAPER_BLUE + INK_WHITE,
	PAPER_GREEN + INK_WHITE,
	PAPER_CYAN + INK_WHITE,
	PAPER_RED + INK_WHITE,
	PAPER_MAGENTA + INK_WHITE
	};

// Takes a number and sticks in a string buffer.
void my_itoa(int value, char *buffer, int buffer_len)
	{
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

void draw_player(char x, char y, char frame)
	{
	// Draw the player tile.
	unsigned char *p = TILE_PLAYER + (frame * TILE_BYTES);
	draw_tile((x * 2) + X_OFFSET, (y * 2), p);
	}
	
void draw_level_tile(char x, char y, char b)
	{
	unsigned char *tile = TILE_TILE;
	unsigned char *p = SCREEN_ATTRIBUTES + ((y * 2) * 32) + (x * 2) + X_OFFSET;

	char attr = block_attrs[b - 48]; // Subtract ASCII 0 from block.

	if(attr==7)
		{
		// Drawing a blank.
		tile = backgrounds[backg];
		attr = backg_attr;
		}

	// Fill in 2x2 attributes for tile colour.
	*p = attr;
	p++;
	*p = attr;
	p = p + 31;
	*p = attr;
	p++;
	*p = attr;	

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
	
	draw_player(px, py, 0);
	// Draw remaining blocks.
	draw_text(0, 23, rem_blocks);
	// Colour text.
	rect(INK_YELLOW + PAPER_BLACK, 0, 21, 3, 3);
	rect(INK_RED + PAPER_BLACK, 0, 21, 1, 1);
	// Draw life counter.	
	life_count[2] = 0x30 + lives;
	draw_text(0,21, life_count);
	
	// Draw level count.
	rect(INK_YELLOW + PAPER_BLACK, 29, 21, 3, 3);	
	draw_text(29,21, level_txt);
	my_itoa(level, level_count, 3);
	draw_text(29,23, level_count);
	}

void draw_death()
	{
	// Start at frame 2.
	char frame = 2, i = 0;
	// Set red on black colours at player location.	
	rect(INK_RED + PAPER_BLACK, (px * 2) + X_OFFSET, py * 2, 2, 2);
	// Spin the character around.			
	for(; i < 4; i++)
		{
		draw_player(px, py, frame);
		frame++;
		if(frame > 4)
			frame = 0;
		#asm
		halt
		halt
		halt
		#endasm
		}
	}
	
void move(char x, char y)
	{
	char new_x, new_y, prev;
	int offset;
		
	new_x = px + x;
	new_y = py + y;
	// Don't go out of bounds.
	if(new_x < 0 || new_x==LEVEL_WIDTH || new_y < 0 || new_y==LEVEL_HEIGHT)
		return;

	// Calculate our new offset into the level data.
	offset = (new_y * LEVEL_WIDTH) + new_x;

	// Have we moved onto a blank tile?
	if(level_buffer[offset]==0x30)
		set_state(STATE_DEAD);
	else if(level_buffer[offset]==0x35)
		{
		// Teleport block.
		// Remove teleport.
		level_buffer[offset] = 0x30;
		draw_level_tile(new_x, new_y, 0x30);	
		
		offset = 0;
		new_x = 0;
		new_y = 0;
		// Find area to teleport to (first 0x34 tile in level?).
		while(level_buffer[offset]!=0x34)
			{
			new_x++;
			if(new_x==LEVEL_WIDTH)
				{
				new_x = 0;
				new_y++;
				}
			offset++;
			}
		}

	offset = (py * LEVEL_WIDTH) + px;
	prev = level_buffer[offset];	
	
	// Try and decrement the tile we were on.
	if(prev < 0x34)
		{
		// We can decrement this tile.
		prev--;
		level_buffer[offset] = prev;
		if(state!=STATE_DEAD && prev==0x30)
			{
			remaining--;
			my_itoa(remaining, rem_blocks, 3);
			draw_text(0,23, rem_blocks);
			if(!remaining)
				{
				set_state(STATE_LEVELUP);
				}
			}
		}

	// Erase our previous player position by redrawing the tile at that position.
	draw_level_tile(px, py, prev);
	
	px = new_x;
	py = new_y;
	
	draw_player(px, py, 0);
	
	if(state!=STATE_DEAD)
		// Play movement sound.
		sound_effect(200,20);
	}
	
void set_state(char new_state)
	{
	state = new_state;
	// Reset interrupt timer.
	interrupt_timer = 0;
	}
	
void init_level(int level)
	{
	int copy = 0;
	struct level_data *current_level;
	char *data;

	if(level > MAX_LEVELS)
		{
		// Completed last level. Should probably do something better here. :)
		// Just wrap round to the start again.
		level = 1;
		}

	current_level = &levels[level - 1];
	data = current_level->data;		
		
	px = current_level->start_x;
	py = current_level->start_y;
	backg = current_level->backg;
	backg_attr = current_level->attr;
	remaining = 0;

	// Set background colour for this level.
	rect(backg_attr, 4, 0, LEVEL_WIDTH * 2, 24);

	// Copy level to our buffer.
	for(; copy < LEVEL_BUFFER_SIZE; copy++)
		{
		level_buffer[copy] = data[copy];
		switch(data[copy])
			{
			case 0x30:
			case 0x34:
			case 0x35:
				break;
			default:
				remaining++;
			}
		}
	
	remaining--;
	my_itoa(remaining, rem_blocks, 3);
	}

void do_init()
	{
	cls(INK_BLACK + PAPER_BLACK);
		
	switch(menu_option)
		{
		case MENU_KEYS:
			{
			// Even though we're not using fire it has to be initialised otherwise
			// the keyboard handling doesn't work.
			k.fire = in_LookupKey('');
			k.left  = in_LookupKey(keys[2]);	// defaults to 'o';
			k.right = in_LookupKey(keys[3]);	// defaults to 'p';
			k.up    = in_LookupKey(keys[0]);	// defaults to 'q';
			k.down  = in_LookupKey(keys[1]);	// defaults to 'a';
	
			joystick = (void *)in_JoyKeyboard;
			}
			break;
			
		case MENU_KEMPSTON:
			{	
			joystick = (void *)in_JoyKempston;
			}
			break;

		case MENU_SINCLAIR1:
			{	
			joystick = (void *)in_JoySinclair1;
			}
			break;

		case MENU_SINCLAIR2:
			{
			joystick = (void *)in_JoySinclair2;
			}
			break;
		}

	lives = LIVES;
	}

// IM2 function.	
M_BEGIN_ISR(isr)
	{
	switch(state)
		{
		case STATE_MENU:
			{
			interrupt_timer++;
			if(interrupt_timer==1)
				{
				update_menu_border();
				interrupt_timer = 0;
				}
			}
			break;
			
		case STATE_PLAYING:
			{
			interrupt_timer++;

			if(interrupt_timer==250)
				// Make the player blink.
				draw_player(px, py, 1);
			if(interrupt_timer==255)
				{
				// Redraw normal player image.
				draw_player(px, py, 0);
				interrupt_timer = 0;
				}
			}
			break;
			
		case STATE_GAMEOVER:
			{
			interrupt_timer++;
			if(interrupt_timer==200)
				set_state(STATE_MENU);
			}
			break;
		}
		
	}
M_END_ISR	
	
void interrupts(char install)
	{
	unsigned short *p;

	if(install)
		{
		// Install interrupt handler.
		p = INTERRUPT_JUMP + 1;
		// Copy interrupt function address to $c5c5 + 1.
		*p = isr;
		#asm
		di
			
		// Create our 256 interrupt table.
		ld hl, INTERRUPT_TABLE
		ld (hl), $c5
		ld de, INTERRUPT_TABLE + 1
		ld bc, 257
		ldir		
		
		// Put a jp instruction to our interrupt function at $c5c5
		ld hl,$c5c5
		ld a, 195
		ld (hl),a

		// Save I register (primarily so we can restore interrupts and go back to basic).
		ld a, i
		ld (INTERRUPT_STORE),a

		// Switch to IM2.
		ld a,$c4
		ld i, a
		im 2
		ei
		#endasm
		}
	else
		{
		#asm		
		di
		//  Restore old I.
		ld a,(INTERRUPT_STORE)
		ld i,a
		im 1
		ei
			
		#endasm
		}
	}

void do_code()
	{
	int len = 0, ascii;
	char code[LEVEL_CODE_SIZE];

	cls(INK_WHITE + PAPER_BLACK);
	
	for(;len <=LEVEL_CODE_SIZE; len++)
		code[len] = 0x0;

	// Clear any proceeding key press.
	while(in_GetKey());
		
	len = 0;
		
	set_state(STATE_CODE);
		
	draw_text(7, CODE_Y_POS - 2,"ENTER LEVEL CODE:");
	rect(INK_WHITE + PAPER_BLUE, CODE_X_POS, CODE_Y_POS, LEVEL_CODE_SIZE - 1, 1);
		
	while(1)
		{
		ascii = in_GetKey();
		if(ascii)
			{
			// In lower case a-z range?
			if(ascii > 0x60 && ascii < 0x7b)
				// Shift down to UPPER case.
				ascii = ascii - 0x20;
			
			// In UPPER case A-Z range?
			if(ascii > 0x40 && ascii < 0x5b && len < LEVEL_CODE_SIZE - 1) 
				{
				// Add character to code.
				if(len < LEVEL_CODE_SIZE - 1)
					{
					code[len]= ascii;
					len++;
					
					draw_text(CODE_X_POS, CODE_Y_POS, code);
					}
				}
				
			if(ascii==0xc && len >=0)
				{
				// Pressed DELETE.
				if(len)
					len--;
					
				code[len]= 0;
				draw_text(CODE_X_POS + len, CODE_Y_POS, " ");
				}
				
			if(ascii==0xd)
				// Pressed ENTER.
				break;
			
			// Typing echo noise (also adds a delay which makes the typing
			// less twitchy.
			sound_effect(250,30);
			}
			
			
		}
		
	// Try and match our code.
	level = find_level_from_code(code);
	}
	
main()
	{
	char wait = 0, i;
	unsigned char direction;
				
	border(0);
	// Set intial menu option to keyboard.
	menu_option = MENU_KEYS;
	// Initialise keyboard handling.
	in_GetKeyReset();
		
	set_state(STATE_MENU);
	
	// Install IM2 handler.
	interrupts(1);

	while(1)
		{			
		while(state==STATE_MENU)
			{
			do_menu();
			if(state==STATE_REDEFINE)
				do_redefine_keys();
			else
				break;
			}
			
		do_code();
						
		do_init();

		set_state(STATE_PLAYING);
			
		wait = 0;
		
		while(lives)
			{
			init_level(level);

			draw_level();
				
			while(state==STATE_PLAYING)
				{
				if(!wait)
					{
					direction = (joystick)(&k);
					if(direction==in_UP)
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
					draw_death();
						
					// Lose a life!					
					sound_effect(600,250);
						
					lives--;
					set_state(STATE_PLAYING);
					rect(INK_BLACK + PAPER_BLACK, 4, 0, LEVEL_WIDTH * 2, 24);	
					}
					break;
					
				case STATE_LEVELUP:
					{
					for(i = 0; i < 6;i++)
						sound_effect(160,120);
					fade();
					
					if(level!= MAX_LEVELS)
						{
						cls(INK_WHITE + PAPER_BLACK);
						draw_text(X_OFFSET, 12, "LEVEL CODE:    ");
						draw_text(19, 12, get_level_code(level));
							
						// Wait for a keypress.
						while(!in_GetKey());
						}
						
					level++;


					cls(INK_BLACK + PAPER_BLACK);
					set_state(STATE_PLAYING);	
					}
					break;
				}
			}
			
		cls(INK_WHITE + PAPER_BLACK);

		draw_text(10,10, game_over);

		// Interrupt will set us back to STATE_MENU.
		set_state(STATE_GAMEOVER);
		while(state==STATE_GAMEOVER);
		}
		
	interrupts(0);
	}
	

