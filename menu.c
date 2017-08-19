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

#include "assets.h"
#include "graphics.h"
#include "levels.h"
#include "main.h"
#include "menu.h"
#include "sound.h"

#define KEYS_LENGTH 	5
#define KEYS_IN_USE	4

unsigned int keys[KEYS_LENGTH] = {'q', 'a', 'o', 'p', ' '};
unsigned char menu_option;
unsigned char border_attr, border_x, border_y;

char scroll_text[]="WALKABOUT - CODE, GFX AND PLIP PLOP MUSIC BY BOB FOSSIL. LEVELS BY HONX. MADE WITH Z88DK.        ";
char *scroll_char;
unsigned char scroll_rem = 0;

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

void do_redefine_keys()
	{		
	unsigned char i = 0, ii = 0, ok = 0, ty = 10;
	char key[2];
	unsigned int ascii = 0;
	const char *descs[4] =
		{
		"UP",
		"DOWN",
		"LEFT",
		"RIGHT"
		};

	key[1] = 0;
		
	// Clear existing keys
	for(i = 0; i < KEYS_IN_USE; i++)
		keys[i] = 0;
	i = 0;
	cls(INK_WHITE + PAPER_BLACK);

	// Show entered keys in a different colour.
	rect(INK_YELLOW + PAPER_BLACK, 20, 10, 1, 8);

	// Clear any proceeding key press.
	in_wait_nokey();

	while(i < KEYS_IN_USE)
		{
		draw_text(10, ty, (char *)descs[i]);

		ok = 0;
		while(!ok)
			{
			// Get a key press.
			ascii = 0;
			ii = 0;

			in_wait_key();
			ascii = in_inkey();
			in_wait_nokey();
				
//			if(ascii[0] > 0x60 && ascii[0] < 0x7b)
//				// Shift down to UPPER case.
//				ascii[0] = ascii[0] - 0x20;	

			// Check it's not already been added.
			while(ii < KEYS_IN_USE)
				{
				if(keys[ii]==ascii)
					break;
				ii++;
				}
								
			if(ii==KEYS_IN_USE)
				{
				key[0] = (char)ascii;
				draw_text(20, ty, (char *)&key);
				ok = 1;
				
				sound_effect(250,30);
				}
			}
			
		keys[i] = ascii;
		i++;
		ty = ty + 2;
		}

	set_state(STATE_MENU);
	}
	
unsigned char do_code()
	{
	unsigned char len = 0;
	int ascii = 0;
	unsigned char code[] = "         ";

	cls(INK_WHITE + PAPER_BLACK);
	
	// Clear any proceeding key press.
	in_wait_nokey();
	set_state(STATE_CODE);
		
	draw_text(7, MENU_CODE_Y_POS - 2,"ENTER LEVEL CODE:");
	rect(INK_WHITE + PAPER_BLUE, MENU_CODE_X_POS, MENU_CODE_Y_POS, LEVEL_CODE_SIZE - 1, 1);
	len = 0;

	while(1)
		{
		in_wait_key();
		ascii = in_inkey();
		in_wait_nokey();

		if(ascii)
			{
			// In lower case a-z range?
			if(ascii > 0x60 && ascii < 0x7b)
				// Shift down to UPPER case.
				ascii = ascii - 0x20;
			
			// In UPPER case A-Z range?
			if(ascii > 0x40 && ascii < 0x5b)
				{
				// Add character to code.
				if(len < (LEVEL_CODE_SIZE - 1))
					{
					code[len]= ascii;
					len++;

					draw_text(MENU_CODE_X_POS, MENU_CODE_Y_POS, code);
					}
				}
			else if(ascii==0xc)// && len >=0)
				{
				// Pressed DELETE.
				if(len)
					len--;
					
				code[len]= 0;
				draw_text(MENU_CODE_X_POS + len, MENU_CODE_Y_POS, " ");
				}
				
			else if(ascii==0xd)
				// Pressed ENTER.
				break;
			
			// Typing echo noise (also adds a delay which makes the typing
			// less twitchy.
			sound_effect(250,30);
			}
		}
		
	// Try and match our code.
	return find_level_from_code(code);
	}	

void do_menu()
	{
	unsigned char new_menu_option, i;
	unsigned char *character, *p;

	// Reset scroller.
	scroll_char = (char *)&scroll_text[0];
	scroll_rem = 0;
	
	clear_scroll_buffer();		
		
	cls(INK_YELLOW + PAPER_BLACK);

	border_attr = INK_YELLOW + PAPER_BLACK;
	border_x = 0;
	border_y = 0;
		
	// Draw a pretty border :).
	for(i = 0; i < 24;i=i+2)
		{
		draw_tile(0, i, (unsigned char *)backgrounds[2]);
		draw_tile(30, i, (unsigned char *)backgrounds[2]);
		}
	for(i = 2; i < 30;i=i+2)
		{
		draw_tile(i, 0, (unsigned char *)backgrounds[2]);
		draw_tile(i, 22, (unsigned char *)backgrounds[2]);
		}
		
	rect(INK_RED + PAPER_BLACK, 9, 4, 13, 1);
	rect(INK_YELLOW + PAPER_BLACK, 11, 4, 9, 1);

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
		if(!scroll_rem)
			{
			// Load character into buffer.
			i = (*scroll_char) - 0x20;
			p = (unsigned char *)SCROLL_CHAR;
			character = (unsigned char *)(FONT + (i * 8));
			i = 0;
						
			for(; i < 8; i++)
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
			do_scroll();
			scroll_rem--;
			}				
					
		if(*scroll_char==0)
			// Reset the scroll.
			scroll_char = (char *)&scroll_text[0];

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
