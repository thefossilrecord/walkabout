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
