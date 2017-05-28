#ifndef LEVELS_H
#define LEVELS_H

#define MAX_LEVELS		20
#define LEVEL_WIDTH		12
#define LEVEL_HEIGHT		12
#define LEVEL_SIZE		(LEVEL_WIDTH * LEVEL_HEIGHT)
#define LEVEL_BUFFER_SIZE 	LEVEL_SIZE + 1
#define MAX_TILES 		6

struct level_data
	{
	char name[33];
	char data[LEVEL_BUFFER_SIZE]; // 144 + 1
	char start_x;
	char start_y;
	};
	
extern struct level_data levels[MAX_LEVELS];
	
#endif	