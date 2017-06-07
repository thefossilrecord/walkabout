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

char scroll_text[]="WALKABOUT - CODE, GFX AND PLIP PLOP MUSIC BY BOB FOSSIL. LEVELS BY HONX. MADE WITH Z88DK.        ";
char *scroll_char;
char scroll_rem = 0;

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
	unsigned char *character, *p;

	// Reset scroller.
	scroll_char = &scroll_text[0];
	scroll_rem = 0;
	
	clear_scroll_buffer();		
		
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
		//do_scroll();		
		if(!scroll_rem)
			{
			// Load character into buffer.
			i = (*scroll_char) - 0x20;
			p = SCROLL_CHAR;	
			character = (unsigned char *)(FONT + (i * 8));
						
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
			do_scroll();
			scroll_rem--;
			}				
					
		if(*scroll_char==0)
			// Reset the scroll.
			scroll_char = &scroll_text[0];	

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
