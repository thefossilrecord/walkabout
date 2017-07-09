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

#ifndef MENU_H
#define MENU_H

#define MENU_OPTIONS_Y_POS	12
#define MENU_CODE_X_POS		11
#define MENU_CODE_Y_POS		12

#define MENU_START		0
#define MENU_KEYS		1
#define MENU_KEMPSTON		2
#define MENU_SINCLAIR1		3
#define MENU_SINCLAIR2		4
#define MENU_REDEFINE		5
#define MENU_NO_SELECTION	9

#define SCROLL_BUFFER 60672
#define SCROLL_CHAR 60662
#define SCROLL_WIDTH 24
#define SCROLL_HEIGHT 8

extern char keys[];
extern char menu_option;

void do_menu();
void do_redefine_keys();
unsigned char do_code();
void update_menu_border();

// asm
extern void clear_scroll_buffer();
extern char check_menu_keys();
extern char do_scroll();

#endif