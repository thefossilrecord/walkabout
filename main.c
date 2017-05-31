#include <input.h>
#include <spectrum.h>
#include <im2.h>

#include "graphics.h"
#include "levels.h"
#include "sound.h"

/*

TO DO:

- Scroller / effects on menu?
- Add backgrounds and colours to all levels.
- Replace menu asm input with C keyboard?

*/

/*

MEMORY MAP:

25000 - 25767 : Font

25768 - 25783 : Player Sprite
25784 - 25799 : Player Sprite Frame 2

25800 -       : Tiles
32768 -       : Code
50176 - 50432 : IM2 Table
50433	      : Previous I value
50629 - 50631 : Interrupt jump instruction
*/

#define FONT 			25000
#define TILES 			25768
#define INTERRUPT_TABLE		0xc400	// 50176
#define INTERRUPT_JUMP		0xc5c5
#define INTERRUPT_STORE		50433

#define TILE_BYTES		32

#define TILE_PLAYER		TILES
#define TILE_PLAYER2		TILE_PLAYER + TILE_BYTES
#define TILE_PLAYER_R90		TILE_PLAYER2 + TILE_BYTES
#define TILE_PLAYER_R180	TILE_PLAYER_R90 + TILE_BYTES
#define TILE_PLAYER_R270	TILE_PLAYER_R180 + TILE_BYTES
#define TILE_TILE		TILE_PLAYER_R270 + TILE_BYTES

#define TILE_BACKGROUND_BLANK	TILE_TILE + TILE_BYTES
#define TILE_BACKGROUND_SQUARES	TILE_BACKGROUND_BLANK + TILE_BYTES
#define TILE_BACKGROUND_STAR	TILE_BACKGROUND_SQUARES + TILE_BYTES
#define TILE_BACKGROUND_MESH	TILE_BACKGROUND_STAR + TILE_BYTES
#define TILE_BACKGROUND_HEART	TILE_BACKGROUND_MESH + TILE_BYTES
#define TILE_BACKGROUND_STRIPES	TILE_BACKGROUND_HEART + TILE_BYTES

#define MAX_BACKGROUND	6

#define X_OFFSET	4
#define KEYBOARD_WAIT	10
#define LIVES		3
#define START_LEVEL	1

#define STATE_MENU	1
#define STATE_CODE	2
#define STATE_PLAYING	3
#define STATE_DEAD	4
#define STATE_LEVELUP	5
#define STATE_GAMEOVER	6

#define INPUT_KEYS	1
#define INPUT_KEMPSTON	2
#define INPUT_SINCLAIR1	3
#define INPUT_SINCLAIR2	4

struct in_UDK k;
void *joystick;
char px, py, state, lives, level, remaining, backg, backg_attr, input, scroll_rem;
char level_buffer[LEVEL_BUFFER_SIZE];
char game_over[]="GAME OVER!";
char rem_blocks[4]="000";
char life_count[4]="^:?";
char level_txt[4]="LVL";
char level_count[4]="000";
char lookup[]= "0123456789";
char scroll_text[]="WALKABOUT - ALL CODE, GFX AND PLIP PLOP MUSIC BY BOB FOSSIL.";
char *scroll_char;
	
int interrupt_timer = 0;
unsigned char *backgrounds[MAX_BACKGROUND]=
	{
	TILE_BACKGROUND_BLANK,
	TILE_BACKGROUND_SQUARES,
	TILE_BACKGROUND_STAR,
	TILE_BACKGROUND_MESH,
	TILE_BACKGROUND_HEART,
	TILE_BACKGROUND_STRIPES,
	};

// black, blue, green, cyan, red, magenta
char block_attrs[MAX_TILES]={0 * 8 + 7, 1 * 8 +7, 4 * 8 + 7, 5 * 8 + 7, 2 * 8 + 7, 3 * 8 + 7};

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

/*
// Waits till any key is pressed. Borrowed from the ROM KEY_SCAN routine at $028e.
// https://skoolkid.github.io/rom/asm/028E.html	
void any_key()
	{
	#asm
		
	ld l,$2f 	// The initial key value for each line will be +2F, +2E,..., +28. (Eight lines)
	ld bc,$fefe 	// C=port address, B=counter.	
	
check_line:
	in a,(c) 	// Read from the port specified.
	cpl 		// A pressed key in the line will set its respective bit, from bit 0 (outer key) to bit 4 (inner key).
	and $1f
	jr nz,end	// Jump forward if one of the five keys in the line are being pressed.

	dec l	 	// Move to next line.
		
	rlc b 		// The counter is shifted and the jump taken if there are still lines to be scanned.
	jr c,check_line

	end:

	#endasm
	}	
*/

// Draw a 16x16 tile.	
void draw_tile(char x, char y, unsigned char *tile)
	{
	unsigned char *p = zx_cyx2saddr(y, x);//addr_from_coords(x,y);//
	draw_block2(p, tile);
	p = zx_cyx2saddr(y+1, x);//addr_from_coords(x,y+1);
	draw_block2(p,tile + 16);
	}
	
void draw_text(char x, char y, char *text)
	{
	unsigned char *p = zx_cyx2saddr(y,x);//	addr_from_coords(x,y);//
	while(*text)
		{
		char offset = (*text) - 0x20;
		unsigned char *character = FONT + (offset * 8);

		draw_block(p, character);
		p++;
		text++;
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
		state = STATE_DEAD;
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
				state = STATE_LEVELUP;
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
		
	switch(input)
		{
		case INPUT_KEYS:
			{
			// Even though we're not using fire it has to be initialised otherwise
			// the keyboard handling doesn't work.
			k.fire = in_LookupKey('');
			k.left  = in_LookupKey('o');
			k.right = in_LookupKey('p');
			k.up    = in_LookupKey('q');
			k.down  = in_LookupKey('a');
	
			joystick = in_JoyKeyboard;
			}
			break;
			
		case INPUT_KEMPSTON:
			{	
			joystick = in_JoyKempston;
			}
			break;

		case INPUT_SINCLAIR1:
			{	
			joystick = in_JoySinclair1;
			}
			break;

		case INPUT_SINCLAIR2:
			{
			joystick = in_JoySinclair2;
			}
			break;
		}

	lives = LIVES;
	}

char check_menu_keys()
	{
	#asm
	ld hl,9		// 9 means we didn't select anything.
		
	// Test for 0.
	ld bc, 61438	// (6, 7, 8, 9, 0)
	in a,(c)
	rra
	jr c, check
	ld hl, 0
	jr exit
	
check:		
	ld bc, 63486
	in a,(c)
	rra
		
	jr c, check1
	ld hl, INPUT_KEYS
	jr exit
check1:
	rra
		
	jr c, check2
	ld hl, INPUT_KEMPSTON
	jr exit
check2:
	rra
		
	jr c, check3
	ld hl, INPUT_SINCLAIR1
	jr exit
check3:	
	rra
		
	jr c, exit
	ld hl, INPUT_SINCLAIR2

exit:
		
	#endasm
	}

#define MENU_OPTIONS_LINE	12
/*
#define SCROLL_BUFFER 60160
#define SCROLL_CHAR 60152
#define SCROLL_WIDTH 24
	
void do_scroll()
	{
	char offset, i, ii,pp;
	int v;
	unsigned char *character, *p = SCROLL_CHAR;
		
	if(!scroll_rem)
		{
		// Load character into buffer.
		offset = (*scroll_char) - 0x20;
		character = (unsigned char *)(FONT + (offset * 8));
					
		for(i= 0; i < 8; i++)
			{
			*p = *character;
			p++;
			character++;
			}
			
		scroll_rem = 8;
		scroll_char++;
		}
	else
		{			
		#asm
		ld b,8
		ld hl,SCROLL_CHAR
		ld de,SCROLL_BUFFER + 23
			
	scrollchar:
		ld a,(hl)	// Load character into a and shift left
		//rla
		ld a,170
		ld (hl),a
		jr nc, noset
	
		ld (de),a	// Load last character of buffer into a
		or 1		// Set least significant bit.
		ld (de),a	// Put back into buffer.
		jr nextline		
	noset:	
	//	ld (hl),a
		and 254
		ld (de),a
		
//	nextline:		
//		ld a,24		// Move to next line in buffer.
//		add a,e
//		ld e,a
//		dec b
//		inc hl		// Next line in character
//
//		jr nz, scrollchar
//	
//		ld b,8
//		
//		ld hl,SCROLL_BUFFER + 22;	
//	aaa1:
//		push bc
//		
//		ld b, SCROLL_WIDTH - 1
//	aaa2:	
//		
//		ld a,(hl)
//		rla
//		ld (hl),a
//		jr nc, noset1
//		dec hl
//		ld a,(hl)
//		or 1
//		ld (hl),a
//	
//	noset1:
//		dec hl
//		dec b
//		jr nz,aaa2
//		
//		
//		ld de,46
//		add hl,de

//	hhhh:
//		jr hhhh


		pop bc
		dec b
		jr nz,aaa1
		
		#endasm

		
//		for(i = 0; i < 8; i++)
//			{
//			p = (unsigned char *)(SCROLL_BUFFER + 22 + (i*SCROLL_WIDTH));
//			for(ii=0; ii < SCROLL_WIDTH -1; ii++)
//				{
//				v = *p;
//				v = v << 1;					
//				*p = (v & 0x7f);
//				p--;
//				if(v > 128)
//					*p = *p | 1;
//				}
//			}
//	
		


		scroll_rem--;
		}
	
	#asm
		
	halt

	#endasm
		
	character = (unsigned char *)SCROLL_BUFFER;
	for(i = 0; i < 8;i++)
		{
		p = (unsigned char *)(20612 + (i * 256));
		for(ii = 0; ii < SCROLL_WIDTH; ii++)
			{
			*p = *character;
			character++;
			p++;
			}
		}
				
	if(*scroll_char==0)
		// Reset the scroll.
		scroll_char = &scroll_text[0];
		
	}
*/

char do_menu()
	{
	char new_input, i;
	
	cls(INK_YELLOW + PAPER_BLACK);
		
//	scroll_char = &scroll_text[0];
//	scroll_rem = 0;
	
	// Draw a pretty border :).
	for(i = 0; i < 24;i=i+2)
		{
		draw_tile(0, i, backgrounds[2]);	
		draw_tile(30, i, backgrounds[2]);	
		}
	for(i = 2; i < 30;i=i+2)
		{
		draw_tile(i, 0, backgrounds[2]);	
		draw_tile(i, 22, backgrounds[2]);	
		}
		
	draw_text(9,4, "^ WALKABOUT ^");
	draw_text(9,6, "BY BOB FOSSIL");
	draw_text(7,(MENU_OPTIONS_LINE - 2), "0: PLAY GAME");
	draw_text(7, MENU_OPTIONS_LINE, "1: KEYBOARD (QAOP)");
	draw_text(7, MENU_OPTIONS_LINE+1, "2: KEMPSTON");
	draw_text(7, MENU_OPTIONS_LINE+2, "3: SINCLAIR PORT 1");
	draw_text(7, MENU_OPTIONS_LINE+3, "4: SINCLAIR PORT 2");
		
	rect(INK_WHITE + PAPER_BLACK, 7, (MENU_OPTIONS_LINE - 2), 18, 6);
	rect(INK_WHITE + PAPER_BLUE, 7, (MENU_OPTIONS_LINE - 1) + input, 18, 1);	

	while(1)
		{
		new_input = check_menu_keys();	
		if(new_input!=9)
			{
			// We pressed a key.
			if(!new_input)
				return;
			if(input!=new_input)
				{
				// Remove old highlight.
				rect(INK_WHITE + PAPER_BLACK, 7, (MENU_OPTIONS_LINE - 1) + input,18,1);
				// Draw new one.
				rect(INK_WHITE + PAPER_BLUE, 7, (MENU_OPTIONS_LINE - 1) + new_input,18,1);
				input = new_input;
				}
			}

		//do_scroll();
		}	
	}
	
// IM2 function.	
M_BEGIN_ISR(isr)
	{
	switch(state)
		{
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

		// Save I register (primarily so we can go back to basic).
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

	
uchar in_KeyDebounce = 1;       // no debouncing
uchar in_KeyStartRepeat = 0;   // wait 20/50s before key starts repeating
uchar in_KeyRepeatPeriod = 0;//10;  // repeat every 10/50s
uint in_KbdState;               // reserved

#define CODE_X_POS	11
#define CODE_Y_POS	12
	
void do_code()
	{
	int len = 0, ascii;
	char code[LEVEL_CODE_SIZE];

	cls(INK_WHITE + PAPER_BLACK);
	
	for(;len <=LEVEL_CODE_SIZE; len++)
		code[len] = 0x0;
		
	len = 0;
		
	set_state(STATE_CODE);
		
	draw_text(7,10,"ENTER LEVEL CODE:");
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
				code[len]= ascii;
				if(len < LEVEL_CODE_SIZE - 2)
				len++;
					
				draw_text(CODE_X_POS, 12, code);
				}
				
			if(ascii==0xc && len >=0)
				{
				// Pressed DELETE.
				if(len)
					len--;
					
				code[len]= 0;
				draw_text(CODE_X_POS + len, 12, " ");
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
	input = INPUT_KEYS;
	in_GetKeyReset();
		
	set_state(STATE_MENU);
	
	// Install IM2 handler.
	interrupts(1);

	while(1)
		{		
		do_menu();
			
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
					
					cls(INK_WHITE + PAPER_BLACK);
					draw_text(X_OFFSET, 12, "LEVEL CODE:    ");
					draw_text(19, 12, get_level_code(level));
						
					level++;

					// Wait for a keypress.
					while(!in_GetKey());

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
	

