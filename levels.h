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

#ifndef LEVELS_H
#define LEVELS_H

#define MAX_LEVELS		20
#define LEVEL_WIDTH		12
#define LEVEL_HEIGHT		12
#define LEVEL_SIZE		(LEVEL_WIDTH * LEVEL_HEIGHT)
#define LEVEL_BUFFER_SIZE 	LEVEL_SIZE + 1
#define LEVEL_CODE_SIZE		10
#define MAX_TILES 		6

struct level_data
	{
	char code[LEVEL_CODE_SIZE];
	char data[LEVEL_BUFFER_SIZE]; // 144 + 1
	char start_x;
	char start_y;
	char attr;
	char backg;
	};
	
extern struct level_data levels[MAX_LEVELS];
	
char * get_level_code(char level);
int find_level_from_code(char *code);

#endif	