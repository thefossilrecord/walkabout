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

; Adapted from article by Jonathan Cauldwell
; https://chuntey.wordpress.com/2013/02/28/how-to-write-zx-spectrum-games-chapter-3/

SECTION code_user
PUBLIC _sound_effect

_sound_effect:

	; void sound_effect(int pitch, char bend)

IFDEF __SCCZ80

	pop hl			; return address
	pop bc			; bc = bend
	ex (sp),hl		; hl = pitch, return address back on stack

ENDIF

IFDEF __SDCC

	pop af       		; return address
	pop hl       		; hl = pitch
	pop bc       		; bc = bend
	push af      		; return address back on stack

ENDIF

	ld b,c			; popped bend into c, so put it into b for the djnz
loop:
	push bc
	push hl			; store pitch.
	ld de, 1		; very short duration.
	call 949		; ROM beeper routine.
	pop hl			; restore pitch.
	inc hl			; pitch going up.
	pop bc
	djnz loop		; repeat.

	ret

