#ifndef SNAKE_H
#define SNAKE_H

#define SNAKE_HEAD 0xF800 
#define SNAKE_BODY 0xFFFF
#define SNAKE_FOOD 0x04FF
#define SNAKE_BG 0x0000

#define SNAKE_GAME_OVER 0
#define SNAKE_MOVE_OK 1

typedef struct snake
{
	char lastDir;
	char direction;
	unsigned char body[600];
	unsigned int length;
	unsigned short shadow[2];

} snake;

snake snakeInit();
int makeMove(snake* s, char direction);
int updateSnakePosition(snake* s);

#endif
