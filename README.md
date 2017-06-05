# walkabout

A ZX Spectrum port of an old PC game POD which had been ported to the Commodore Amiga. Coded in C and z80 assembly.

Gameplay wise, you need to move around and remove all the tiles from the screen to proceed to the next level. Blue tiles remove after you move off them, green tiles change into blue ones when you move off them and cyan tiles turn into green ones when you move off them. Magenta tiles are one time teleporters which take you back to the first red tile in the level. Red tiles are safe zones and won't disappear however many times you move over them. You lose a life if you step into empty space. There are currently twenty levels. You get a level code when you complete a level.

Additional credits:

* z88dk for the compiler
* Honx for the levels which I converted over.
* Jonathan Cauldwell for his sound effect code which I adapted.
* http://listofrandomwords.com - for the level codes.

To build from source you'll need:

* z88dk https://www.z88dk.org/forum/
* sjasmplus (or a compatible command line z80 assembler of your choice)
* bas2tap https://github.com/andybalaam/bas2tap

**make_walkabout.sh** is a Linux build script. For Windows you'll need to change

`cat basic.tap assets.tap code.tap > walkabout.tap`

to something like

`copy /b basic.tap+assets.tap+code.tap walkabout.tap`

