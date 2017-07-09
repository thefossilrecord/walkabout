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

#ifndef GFX_H
#define GFX_H

#include <arch/zx.h>

#define SCREEN_ATTRIBUTES 	22528
#define ATRR_LINE_WIDTH		32

#define BORDCR			23624

void rect(unsigned char attr, unsigned char x, unsigned char y, unsigned char w, unsigned char h);
void draw_tile(unsigned char x, unsigned char y, unsigned char *tile);
void draw_text(unsigned char x, unsigned char y, char *text);

// asm
extern void cls(int attr) __z88dk_fastcall;
extern void border(char c) __z88dk_fastcall;
extern void fade();
extern void draw_block(unsigned char *address, unsigned char *tile) __z88dk_callee;
extern void draw_block2(unsigned char *address, unsigned char *tile) __z88dk_callee;

#endif