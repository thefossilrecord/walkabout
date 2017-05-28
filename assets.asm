
	OUTPUT "assets.bin"

	ORG 25000
	
	INCBIN "assets/font.bin"	; 25000 - 25767 font

	include "tiles.asm"		; 25768 - ... tiles
