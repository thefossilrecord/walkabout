zcc +zx -vn -startup=31 -clib=new assets.asm music.asm graphics.c graphics.asm levels.c sound.asm main.asm main.c menu.c menu.asm -o game.bin -pragma-include:zpragma.inc
appmake +zx -b game_assets.bin -o assets.tap --blockname assets.bin --org 25000 --noloader
appmake +zx -b game.bin -o code.tap --blockname game.bin --org 33200 --noloader
#appmake +zx -b music1.bin -o music.tap --blockname music.bin --org 49152 --noloader
~/dev/c/bas2tap-master/bas2tap -a10 -sWalkabout ./basic.txt basic.tap
cat basic.tap assets.tap code.tap > walkabout.tap
