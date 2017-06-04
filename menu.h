#ifndef MENU_H
#define MENU_H

#define MENU_OPTIONS_Y_POS	12

#define MENU_START		0
#define MENU_KEYS		1
#define MENU_KEMPSTON		2
#define MENU_SINCLAIR1		3
#define MENU_SINCLAIR2		4
#define MENU_REDEFINE		5
#define MENU_NO_SELECTION	9

extern char keys[];
extern char menu_option;

void do_menu();
void do_redefine_keys();
void update_menu_border();

#endif