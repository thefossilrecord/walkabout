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

#include "assets.h"
#include "graphics.h"
#include "main.h"
#include "menu.h"
#include "sound.h"

#define KEYS_LENGTH 	4

char keys[]="qaop";
char menu_option;
char border_attr, border_x, border_y;

#define SCROLL_BUFFER 60160
#define SCROLL_CHAR 60150
#define SCROLL_WIDTH 24
#define SCROLL_HEIGHT 8

char scroll_text[]="WALKABOUT - CODE, GFX AND PLIP PLOP MUSIC BY BOB FOSSIL. LEVELS BY HONX. MADE WITH Z88DK.        ";
char *scroll_char;
char scroll_rem = 0;

//unsigned char menu_text[] = 	"^ WALKABOUT \0xde"	// ^ + 0x80
//				"BY BOB FOSSI\0xcc";	// L + 0x80

void update_menu_border()
	{
	rect(border_attr, border_x, border_y, 2, 2);
	if(border_x < 30 && !border_y)
		border_x = border_x + 2;
	else if(border_x==30 && border_y < 22)
		border_y = border_y + 2;
	else if(border_x > 0 && border_y==22)
		border_x = border_x - 2;
	else if(border_x==0 && border_y >0)
		border_y = border_y - 2;
	if(border_x==0 && border_y==0)
		{
		border_attr++;
		if(border_attr > INK_WHITE)
			border_attr = INK_BLUE;
		}	
	}

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
		
		ld hl, SCROLL_BUFFER
		ld de, SCROLL_CHAR
		
		ld b,SCROLL_HEIGHT		

shiftstart:
		push bc
		push de
		ld b, SCROLL_WIDTH - 1
			
		ld a,(hl)	// Don't need to carry over bit for first character on line.
		sla a
		ld (hl),a
			
		ld d, h		// de is now SCROLL_BUFFER - 1
		ld e, l
		inc l		// move to next in SCROLL_BUFFER
		
shiftline:
		ld a,(hl)
		sla a
		ld (hl),a
		
		ld a, (de)
		
		jr nc, shiftnoset

		set 0, a
		jr shiftskip
shiftnoset:
		res 0,a

shiftskip:
		ld (de),a
		inc l
		inc e
			
		djnz shiftline

		// Copy the next set of bits from our character in the buffer (de).
		pop de
		
		dec l		// Move back to the last character on the line.

		ld b,8
addchar:
		ld a,(de)
		sla a
		ld (de),a
		ld a,(hl)
		
		jr nc, addnoset
		
		set 0,a
		jr addskip

addnoset:
		res 0,a
addskip:	
		ld (hl),a

		inc e		// Move to next line in character buffer.
		inc l		// Move on to start of next line.

		pop bc
		djnz shiftstart
			
		#endasm
	
		scroll_rem--;			
		}

	#asm
	
	ld hl,20612
	ld de,SCROLL_BUFFER

	ld b,SCROLL_HEIGHT
	
	halt

linestart:

	push bc
	ld b,SCROLL_WIDTH

linecopy:

	ld a,(de)		// Copy a line from the buffer to the screen
	ld (hl),a
	inc de
	inc l
	djnz linecopy
	
	ld a, l			// Move l back to the start of the line.
	sub SCROLL_WIDTH
	ld l, a
	inc h			// Move h on by 256 bytes to the next line.
	
	pop bc
	djnz linestart

	#endasm
				
	if(*scroll_char==0)
		// Reset the scroll.
		scroll_char = &scroll_text[0];
		
	}

char check_menu_keys()
	{
/*		
	char ascii;
	ascii = in_GetKey();
	switch(ascii)
		{
		case 0x30:		// 0
			{
			ascii = 0;
			break;
			}
			
		case 0x31:		// 1
			{
			ascii = INPUT_KEYS;
			break;
			}

		case 0x32:		// 2
			{
			ascii = INPUT_KEMPSTON;
			break;
			}

		case 0x33:		// 3
			{
			ascii = INPUT_SINCLAIR1;
			break;
			}

		case 0x34:		// 4
			{
			ascii = INPUT_SINCLAIR1;
			break;
			}			
		
		default:
			ascii = 9;
		}
		
	return ascii;
*/		
	#asm
	ld hl, MENU_NO_SELECTION	// Set initial no selection.
		
	// Test for 0.
	ld bc, 61438	// (6, 7, 8, 9, 0)
	in a,(c)
	rra
	jr c, check
	ld hl, MENU_START
	jr exit
	
check:		
	ld bc, 63486	// (5,4,3,2,1)
	in a,(c)
	rra
		
	jr c, check1
	ld hl, MENU_KEYS
	jr exit
check1:
	rra
		
	jr c, check2
	ld hl, MENU_KEMPSTON
	jr exit
check2:
	rra
		
	jr c, check3
	ld hl, MENU_SINCLAIR1
	jr exit
check3:	
	rra
		
	jr c, check4
	ld hl, MENU_SINCLAIR2
	jr exit
check4:
	rra
	
	jr c,exit
	ld hl, MENU_REDEFINE
exit:
		
	#endasm	
	}
	
void do_redefine_keys()
	{		
	char i = 0, ii, ascii[2], ok, ty = 10;
	char *descs[4] = {"UP", "DOWN", "LEFT", "RIGHT"};
	ascii[1] = 0;
	// Clear existing keys
	for(i = 0; i < KEYS_LENGTH; i++)
		keys[i] = 0;
	i = 0;
	cls(INK_WHITE + PAPER_BLACK);

	// Show entered keys in a different colour.
	rect(INK_YELLOW + PAPER_BLACK, 20, 10, 1, 8);

	// Clear any proceeding key press.
	while(in_GetKey());

	while(i < KEYS_LENGTH)
		{
		draw_text(10, ty, descs[i]);

		ok = 0;
		while(!ok)
			{
			// Get a key press.
			ascii[0] = 0;
			ii = 0;
			while(!ascii[0])
				{
				ascii[0] = in_GetKey();
				}
				
//			if(ascii[0] > 0x60 && ascii[0] < 0x7b)
//				// Shift down to UPPER case.
//				ascii[0] = ascii[0] - 0x20;	

			// Check it's not already been added.
			while(ii < KEYS_LENGTH)
				{
				if(keys[ii]==ascii[0])
					break;
				ii++;
				}
				
			if(ii==KEYS_LENGTH)
				{
				draw_text(20, ty, &ascii[0]);
				ok = 1;
				
				sound_effect(250,30);
				}
			}
			
		keys[i] = ascii[0];
		i++;
		ty = ty + 2;
		}

	set_state(STATE_MENU);
	}

void do_menu()
	{
	char new_menu_option, i;

	// Reset scroller.
	scroll_char = &scroll_text[0];
	scroll_rem = 0;
		
	#asm
	xor a
	ld hl, SCROLL_BUFFER
	ld (hl), a
	ld de, SCROLL_BUFFER+1
	ld bc, SCROLL_WIDTH * SCROLL_HEIGHT-1
	ldir
	#endasm		
		
	cls(INK_YELLOW + PAPER_BLACK);

	border_attr = INK_YELLOW + PAPER_BLACK;
	border_x = 0;
	border_y = 0;
		
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
	draw_text(7,(MENU_OPTIONS_Y_POS - 2), "0: PLAY GAME");
	draw_text(7, MENU_OPTIONS_Y_POS, "1: KEYBOARD");// - ");
	//draw_text(21, MENU_OPTIONS_Y_POS, keys);		
	draw_text(7, MENU_OPTIONS_Y_POS+1, "2: KEMPSTON");
	draw_text(7, MENU_OPTIONS_Y_POS+2, "3: SINCLAIR PORT 1");
	draw_text(7, MENU_OPTIONS_Y_POS+3, "4: SINCLAIR PORT 2");
	draw_text(7, MENU_OPTIONS_Y_POS+4, "5: REDEFINE KEYS");
		
	rect(INK_WHITE + PAPER_BLACK, 7, (MENU_OPTIONS_Y_POS - 2), 18, 7);
	rect(INK_WHITE + PAPER_BLUE, 7, (MENU_OPTIONS_Y_POS - 1) + menu_option, 18, 1);	

	while(1)
		{
		do_scroll();
			
		new_menu_option = check_menu_keys();	
		if(new_menu_option!=MENU_NO_SELECTION)
			{
			// We pressed a key.
			if(!new_menu_option)
				return;
			if(new_menu_option==MENU_REDEFINE)
				{
				set_state(STATE_REDEFINE);
				return;
				}
			else
				{
				// Remove old highlight.
				rect(INK_WHITE + PAPER_BLACK, 7, (MENU_OPTIONS_Y_POS - 1) + menu_option,18,1);
				// Draw new one.
				rect(INK_WHITE + PAPER_BLUE, 7, (MENU_OPTIONS_Y_POS - 1) + new_menu_option,18,1);
				menu_option = new_menu_option;
				}
			}
		}	
	}
