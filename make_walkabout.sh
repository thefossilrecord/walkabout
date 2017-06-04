zcc +zx -vn graphics.c levels.c sound.c main.c menu.c -o game.bin -lndos
~/dev/c/sjasmplus/sjasmplus assets.asm
appmake +zx -b assets.bin -o assets.tap --blockname assets.bin --org 25000 --noloader
appmake +zx -b game.bin -o code.tap --blockname game.bin --org 32768 --noloader
~/dev/c/bas2tap-master/bas2tap -a10 -sWalkabout ./basic.txt basic.tap
cat basic.tap assets.tap code.tap > walkabout.tap
