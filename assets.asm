;******************************************************************************
;*
;* Copyright(c) 2017 Bob Fossil. All rights reserved.
;*                                        
;* This program is free software; you can redistribute it and/or modify it
;* under the terms of version 2 of the GNU General Public License as
;* published by the Free Software Foundation.
;*
;* This program is distributed in the hope that it will be useful, but WITHOUT
;* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
;* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
;* more details.
;*
;* You should have received a copy of the GNU General Public License along with
;* this program; if not, write to the Free Software Foundation, Inc.,
;* 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
;*
;*
;******************************************************************************

	OUTPUT "assets.bin"

	ORG 25000

	; 25000 - 25767 : Font

	include "font.asm"

	; 25768 - 25799 : Player Sprite
	; 25800 - 25831 : Player Sprite Frame 2
	; 25832 - 25863 : Player Sprite Rotated 90 degress
	; 25864 - 25895 : Player Sprite Rotated 180 degress
	; 25896 - 25927 : Player Sprite Rotated 270 degress
	; 25928 - 25959 : Level Tile
	; 25960 - 26279 : Background Tiles 1 - 10
	
	include "tiles.asm"


