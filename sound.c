
// Adapted from article by Jonathan Cauldwell
// https://chuntey.wordpress.com/2013/02/28/how-to-write-zx-spectrum-games-chapter-3/
void sound_effect(int pitch, char bend)
	{
	#asm
	
	ld hl,2
	add hl,sp		// skip over return address on stack
	
	ld b,(hl)		// b = bend
	inc hl
	inc hl
	ld e,(hl)		// load pitch into de		
	inc hl
	ld d,(hl)
		
	ex de,hl		// swap pitch into hl
		
loop:
	push bc
	push hl             ; store pitch.
	ld de,1             ; very short duration.
	call 949            ; ROM beeper routine.
	pop hl              ; restore pitch.
	inc hl              ; pitch going up.
	pop bc
	djnz loop           ; repeat.

	#endasm
	}
