#ifndef SNAKE_H
#define SNAKE_H

#define SNAKE_HEAD 0xF800 
#define SNAKE_BODY 0xFFFF
#define SNAKE_FOOD 0x04FF
#define SNAKE_BG 0x0000

#define SNAKE_GAME_OVER 0
#define SNAKE_MOVE_OK 1


/*
	Every odd indexes in body array corresponds to x-coordinates
	Even indexes corresponds to y-coordinates. First two indexes 
	keeps track of snake head, the rest keeps track of the body.
	Shadow keeps track of where the snakes tail was previouse tick,
	is used to clear correct pixel.
*/
typedef struct snake
{
	char lastDir;
	char direction;
	unsigned char body[600];
	unsigned int length;
	unsigned short shadow[2];
} snake;

snake snakeInit();
int makeMove(snake* s, char direction, char eat);
int updateSnakePosition(snake* s, char eat);

#endif
