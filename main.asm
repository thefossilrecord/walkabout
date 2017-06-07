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

SECTION code_user
PUBLIC _break_pressed

_break_pressed:

	halt		; Add a pause here to slow the keyboard scanning down.

	ld hl, 0
	ld a, $7f 	; Form the port address +7FFE and read in a byte.
	in a,($fe)
	rra 		; Examine only bit 0 by shifting it into the carry position.
	ret c		;jr c, done 	; Return if the BREAK key is not being pressed.
	
	ld a,$fe 	; Form the port address +FEFE and read in a byte.
	in a,($fe)
	rra	 	; Again examine bit 0.
	ret c		;jr c, done
	
	ld hl, 1	; BREAK is pressed

	ret