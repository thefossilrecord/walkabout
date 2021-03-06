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

	defb $00, $00, $00, $00, $00, $00, $00, $00	;  
	defb $10, $38, $38, $30, $20, $00, $60, $00	; !
	defb $d8, $d8, $48, $90, $00, $00, $00, $00	; "
	defb $6c, $fe, $6c, $6c, $6c, $fe, $6c, $00	; #
	defb $10, $7c, $d0, $7c, $16, $fc, $10, $00	; $
	defb $62, $a4, $c8, $10, $26, $4a, $8c, $00	; %
	defb $30, $48, $68, $70, $9a, $8c, $72, $00	; &
	defb $c0, $c0, $40, $80, $00, $00, $00, $00	; '
	defb $06, $0c, $18, $18, $18, $0c, $06, $00	; (
	defb $c0, $60, $30, $30, $30, $60, $c0, $00	; )
	defb $10, $92, $54, $38, $54, $92, $10, $00	; *
	defb $00, $30, $30, $fc, $30, $30, $00, $00	; +
	defb $00, $00, $00, $00, $c0, $40, $80, $00	; ,
	defb $00, $00, $00, $7c, $00, $00, $00, $00	; -
	defb $00, $00, $00, $00, $00, $c0, $c0, $00	; .
	defb $02, $04, $08, $10, $20, $40, $80, $00	; /
	defb $7c, $c6, $c6, $c6, $c6, $c6, $7c, $00	; 0
	defb $18, $38, $18, $18, $18, $18, $3c, $00	; 1
	defb $7c, $c6, $06, $0c, $30, $60, $fe, $00	; 2
	defb $7c, $c6, $c6, $1c, $c6, $c6, $7c, $00	; 3
	defb $1c, $3c, $6c, $4c, $cc, $fe, $0c, $00	; 4
	defb $fe, $c0, $fc, $06, $06, $c6, $7c, $00	; 5
	defb $7c, $c6, $c0, $fc, $c6, $c6, $7c, $00	; 6
	defb $fe, $c6, $0c, $18, $30, $30, $30, $00	; 7
	defb $7c, $c6, $c6, $7c, $c6, $c6, $7c, $00	; 8
	defb $7c, $c6, $c6, $7e, $06, $c6, $7c, $00	; 9
	defb $00, $18, $18, $00, $18, $18, $00, $00	; :
	defb $00, $18, $18, $00, $18, $08, $10, $00	; ;
	defb $06, $0c, $18, $30, $18, $0c, $06, $00	; <
	defb $00, $00, $7c, $00, $7c, $00, $00, $00	; =
	defb $c0, $60, $30, $18, $30, $60, $c0, $00	; >
	defb $3c, $66, $66, $0c, $18, $00, $18, $00	; ?
	defb $3c, $42, $99, $a1, $a1, $99, $42, $3c	; @
	defb $38, $6c, $c6, $c6, $fe, $c6, $c6, $00	; A
	defb $fc, $c6, $c6, $fc, $c6, $c6, $fc, $00	; B
	defb $3c, $66, $c0, $c0, $c0, $66, $3c, $00	; C
	defb $f8, $cc, $c6, $c6, $c6, $cc, $f8, $00	; D
	defb $fe, $c0, $c0, $fc, $c0, $c0, $fe, $00	; E
	defb $fe, $c0, $c0, $fc, $c0, $c0, $c0, $00	; F
	defb $3e, $60, $c0, $ce, $c6, $66, $3e, $00	; G
	defb $c6, $c6, $c6, $fe, $c6, $c6, $c6, $00	; H
	defb $7e, $18, $18, $18, $18, $18, $7e, $00	; I
	defb $06, $06, $06, $06, $06, $c6, $7c, $00	; J
	defb $c6, $cc, $d8, $f0, $f8, $dc, $ce, $00	; K
	defb $60, $60, $60, $60, $60, $60, $7e, $00	; L
	defb $c6, $ee, $fe, $fe, $d6, $c6, $c6, $00	; M
	defb $c6, $e6, $f6, $fe, $de, $ce, $c6, $00	; N
	defb $7c, $c6, $c6, $c6, $c6, $c6, $7c, $00	; O
	defb $fc, $c6, $c6, $c6, $fc, $c0, $c0, $00	; P
	defb $7c, $c6, $c6, $c6, $de, $cc, $7a, $00	; Q
	defb $fc, $c6, $c6, $ce, $f8, $dc, $ce, $00	; R
	defb $78, $cc, $c0, $7c, $06, $c6, $7c, $00	; S
	defb $7e, $18, $18, $18, $18, $18, $18, $00	; T
	defb $c6, $c6, $c6, $c6, $c6, $c6, $7c, $00	; U
	defb $c6, $c6, $c6, $ee, $7c, $38, $10, $00	; V
	defb $c6, $c6, $d6, $fe, $fe, $ee, $c6, $00	; W
	defb $c6, $ee, $7c, $38, $7c, $ee, $c6, $00	; X
	defb $66, $66, $66, $3c, $18, $18, $18, $00	; Y
	defb $fe, $0e, $1c, $38, $70, $e0, $fe, $00	; Z
	defb $1e, $18, $18, $18, $18, $18, $1e, $00	; [
	defb $6c, $fe, $fe, $fe, $7c, $38, $10, $00	; \
	defb $f0, $30, $30, $30, $30, $30, $f0, $00	; ]
	
	;defb $38, $6c, $00, $00, $00, $00, $00, $00	; heart (was ^)
	
	defb %00000000
	defb %00110110
	defb %01111111
	defb %01111111
	defb %01111111
	defb %00111110
	defb %00011100
	defb %00001000
	
	defb $00, $00, $00, $00, $00, $00, $fe, $00	; _
	defb $c0, $e0, $20, $00, $00, $00, $00, $00	; `
	defb $00, $00, $78, $0c, $7c, $cc, $7c, $00	; a
	defb $00, $c0, $c0, $f8, $cc, $cc, $f8, $00	; b
	defb $00, $00, $78, $cc, $c0, $cc, $78, $00	; c
	defb $00, $0c, $0c, $7c, $cc, $cc, $7c, $00	; d
	defb $00, $00, $78, $cc, $fc, $c0, $7c, $00	; e
	defb $00, $1c, $30, $7c, $30, $30, $30, $00	; f
	defb $00, $00, $7c, $cc, $7c, $0c, $78, $00	; g
	defb $00, $c0, $c0, $d8, $ec, $cc, $cc, $00	; h
	defb $00, $30, $00, $70, $30, $30, $78, $00	; i
	defb $00, $18, $00, $18, $18, $d8, $70, $00	; j
	defb $00, $c0, $c0, $d8, $f0, $f8, $dc, $00	; k
	defb $00, $70, $30, $30, $30, $30, $30, $00	; l
	defb $00, $00, $fc, $b6, $b6, $b6, $b6, $00	; m
	defb $00, $00, $d8, $ec, $cc, $cc, $cc, $00	; n
	defb $00, $00, $78, $cc, $cc, $cc, $78, $00	; o
	defb $00, $00, $f8, $cc, $cc, $f8, $c0, $00	; p
	defb $00, $00, $7c, $cc, $cc, $7c, $0c, $00	; q
	defb $00, $00, $6c, $7c, $70, $60, $60, $00	; r
	defb $00, $00, $7c, $e0, $78, $1c, $f8, $00	; s
	defb $00, $30, $7c, $30, $30, $30, $1c, $00	; t
	defb $00, $00, $cc, $cc, $cc, $dc, $6c, $00	; u
	defb $00, $00, $cc, $cc, $cc, $78, $30, $00	; v
	defb $00, $00, $c6, $c6, $d6, $7c, $6c, $00	; w
	defb $00, $00, $c6, $ee, $7c, $ee, $c6, $00	; x
	defb $00, $00, $cc, $cc, $7c, $0c, $78, $00	; y
	defb $00, $00, $fc, $18, $30, $60, $fc, $00	; z
	defb $06, $0c, $0c, $18, $0c, $0c, $06, $00	; {
	defb $10, $10, $10, $10, $10, $10, $10, $00	; |
	defb $c0, $60, $60, $30, $60, $60, $c0, $00	; }
	defb $00, $00, $70, $ba, $1c, $00, $00, $00	; ~
	defb $00, $67, $54, $57, $54, $54, $67, $00	; 127
