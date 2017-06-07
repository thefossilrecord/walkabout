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

DEFC MENU_START=0
DEFC MENU_KEYS=1
DEFC MENU_KEMPSTON=2
DEFC MENU_SINCLAIR1=3
DEFC MENU_SINCLAIR2=4
DEFC MENU_REDEFINE=5
DEFC MENU_NO_SELECTION=9

DEFC SCROLL_BUFFER=60160
DEFC SCROLL_CHAR=60150
DEFC SCROLL_WIDTH=24
DEFC SCROLL_HEIGHT=8

SECTION code_user
PUBLIC _clear_scroll_buffer

_clear_scroll_buffer:

	xor a
	ld hl, SCROLL_BUFFER
	ld (hl), a
	ld de, SCROLL_BUFFER + 1
	ld bc, SCROLL_WIDTH * SCROLL_HEIGHT - 1
	ldir
	ret	

PUBLIC _check_menu_keys

_check_menu_keys:

	; char check_menu_keys()

;	char ascii;
;	ascii = in_GetKey();
;	switch(ascii)
;		{
;		case 0x30:		// 0
;			{
;			ascii = 0;
;			break;
;			}
;			
;		case 0x31:		// 1
;			{
;			ascii = INPUT_KEYS;
;			break;
;			}
;
;		case 0x32:		// 2
;			{
;			ascii = INPUT_KEMPSTON;
;			break;
;			}
;
;		case 0x33:		// 3
;			{
;			ascii = INPUT_SINCLAIR1;
;			break;
;			}
;
;		case 0x34:		// 4
;			{
;			ascii = INPUT_SINCLAIR1;
;			break;
;			}			
;		
;		default:
;			ascii = 9;
;		}
;		
;	return ascii;

	ld hl, MENU_NO_SELECTION	; Set initial no selection.
		
	; Test for 0.
	ld bc, 61438			; (6, 7, 8, 9, 0)
	in a,(c)
	rra
	jr c, check
	ld hl, MENU_START
	jr exit
	
check:		
	ld bc, 63486			; (5,4,3,2,1)
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
		
	ret

PUBLIC _do_scroll

_do_scroll:

	ld hl, SCROLL_BUFFER
	ld de, SCROLL_CHAR
	
	ld b, SCROLL_HEIGHT

shiftstart:
	push bc
	push de
	ld b, SCROLL_WIDTH - 1
		
	ld a,(hl)		; Don't need to carry over bit for first character on line.
	sla a
	ld (hl),a
		
	ld d, h			; de is now SCROLL_BUFFER - 1
	ld e, l
	inc l			; move to next in SCROLL_BUFFER

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

	; Copy the next set of bits from our character in the buffer (de).
	pop de
	
	dec l			; Move back to the last character on the line.

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

	inc e			; Move to next line in character buffer.
	inc l			; Move on to start of next line.

	pop bc
	djnz shiftstart

	ld hl,20612
	ld de,SCROLL_BUFFER

	ld b,SCROLL_HEIGHT
	
	halt

linestart:

	push bc
	ld b,SCROLL_WIDTH

linecopy:

	ld a,(de)		; Copy a line from the buffer to the screen
	ld (hl),a
	inc de
	inc l
	djnz linecopy
	
	ld a, l			; Move l back to the start of the line.
	sub SCROLL_WIDTH
	ld l, a
	inc h			; Move h on by 256 bytes to the next line.
	
	pop bc
	djnz linestart

	ret
