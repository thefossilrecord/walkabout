
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


