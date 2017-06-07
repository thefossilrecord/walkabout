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
;******************************************************************************/

DEFC SCREEN_ATTRIBUTES=22528
DEFC BORDCR=23624

SECTION code_user
PUBLIC _cls

_cls:

	; void cls(int attr)
	; fastcall linkage	
	; Erases the screen and sets an initial attribute value.

	ld a, l				; get attr from l

	ld hl, 16384
	ld (hl), 0
	ld de, 16385
	ld bc, 6143
	ldir

	ld hl, SCREEN_ATTRIBUTES
	ld (hl), a
	ld de, SCREEN_ATTRIBUTES + 1
	ld bc, 767
	ldir

	ret

PUBLIC _border

_border:
	; void border(char c)
	; fastcall linkage
		
	ld a, l			; get c from l
	out (254), a
		
	; multiply border by 8 and store in BORDCR system variable
	; otherwise subsequent calls to BEEPER zaps the border colour.
	rlca
	rlca
	rlca
	ld (BORDCR), a	
	
	ret

PUBLIC _fade

_fade:
	; void fade()

	ld a, 48
	ld b, 7

fadeloop:
	push bc
	halt
	halt
	ld hl, SCREEN_ATTRIBUTES
	ld (hl), a
	ld de, SCREEN_ATTRIBUTES + 1
	ld bc, 767
	ldir
	pop bc
	sub 8
	djnz fadeloop
		
	ret

PUBLIC _draw_block

_draw_block:
	; void draw_block(unsigned char *address, unsigned char *tile)
		
;	int index = 0;
;	for(;index < 8; index ++)
;		{
;		*address = *tile;
;		tile++;
;		address = address + 256;
;		}

	ld hl, 2
	add hl, sp		; skip over return address on stack

	ld e, (hl)		; put tile into de
	inc hl			 
	ld d, (hl)
	inc hl
	ld c, (hl)		; put address into bc
	inc hl
	ld b, (hl)
	ld h, b			; copy address into hl
	ld l, c
	ld b, 8			; loop 8 times
		
copy:
	ld a,(de)
	ld (hl),a
	inc de
	inc h
	djnz copy

	ret

PUBLIC _draw_block2

_draw_block2:
	; void draw_block2(unsigned char *address, unsigned char *tile)
		
;	int index = 0;
;	for(;index < 8; index ++)
;		{
;		*address = *tile;
;		tile++;
;		address++;
;		*address = *tile;
;		address = address + 255;
;		tile++;
;		}		
		
	ld hl,2
	add hl,sp		; skip over return address on stack

	ld e,(hl)		; put tile into de
	inc hl			 
	ld d,(hl)
	inc hl
	ld c,(hl)		; put address into bc
	inc hl
	ld b,(hl)
	ld h,b			; copy address into hl
	ld l,c
	ld b,8			; loop 8 times
		
copy2:
	ld a,(de)
	ld (hl),a
	inc de
	inc hl
	ld a,(de)
	ld (hl),a
	inc de
	dec hl
	inc h
	djnz copy2

	ret
