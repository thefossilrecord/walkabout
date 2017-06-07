zcc +zx -vn assets.asm graphics.c graphics.asm levels.c sound.asm main.asm main.c menu.c menu.asm -o game.bin -lndos
appmake +zx -b game_assets.bin -o assets.tap --blockname assets.bin --org 25000 --noloader
appmake +zx -b game.bin -o code.tap --blockname game.bin --org 32768 --noloader
~/dev/c/bas2tap-master/bas2tap -a10 -sWalkabout ./basic.txt basic.tap
cat basic.tap assets.tap code.tap > walkabout.tap
