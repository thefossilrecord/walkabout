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

#ifndef ASSETS_H
#define ASSETS_H

#define FONT 	25000
#define TILES 	25768

#define TILE_BYTES		32

#define TILE_PLAYER		TILES
#define TILE_PLAYER2		TILE_PLAYER + TILE_BYTES
#define TILE_PLAYER_R90		TILE_PLAYER2 + TILE_BYTES
#define TILE_PLAYER_R180	TILE_PLAYER_R90 + TILE_BYTES
#define TILE_PLAYER_R270	TILE_PLAYER_R180 + TILE_BYTES
#define TILE_TILE		TILE_PLAYER_R270 + TILE_BYTES

#define TILE_BACKGROUND_BLANK		TILE_TILE + TILE_BYTES
#define TILE_BACKGROUND_SQUARES		TILE_BACKGROUND_BLANK + TILE_BYTES
#define TILE_BACKGROUND_STAR		TILE_BACKGROUND_SQUARES + TILE_BYTES
#define TILE_BACKGROUND_MESH		TILE_BACKGROUND_STAR + TILE_BYTES
#define TILE_BACKGROUND_TOYBLOCK	TILE_BACKGROUND_MESH + TILE_BYTES
#define TILE_BACKGROUND_STRIPES		TILE_BACKGROUND_TOYBLOCK + TILE_BYTES
#define TILE_BACKGROUND_DOTS		TILE_BACKGROUND_STRIPES + TILE_BYTES
#define TILE_BACKGROUND_DIAMOND		TILE_BACKGROUND_DOTS + TILE_BYTES
#define TILE_BACKGROUND_GRID		TILE_BACKGROUND_DIAMOND + TILE_BYTES
#define TILE_BACKGROUND_NET		TILE_BACKGROUND_GRID + TILE_BYTES

#define MAX_BACKGROUND	10

#ifdef __SDCC
extern unsigned short backgrounds[MAX_BACKGROUND];
#else
extern unsigned char *backgrounds[MAX_BACKGROUND];
#endif

#endif
