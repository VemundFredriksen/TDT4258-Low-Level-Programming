#include <stdlib.h>
#include "snake.h"

snake snakeInit(){
	snake s = {
		.direction = 0,
		
		.body[0] = 2,
		.body[1] = 8,
		.body[2] = 1,
		.body[3] = 8,
		
		.length = 2,
		
		.shadow[0] = 0,
		.shadow[1] = 0
	};
	
	return s;
}

int makeMove(snake* s, char direction, char eat)
{
	if (direction == 0 && s->direction != 2) {
		s->direction = 0;
	}
	else if (direction == 1 && s->direction != 3) {
		s->direction = 1;
	}
	else if (direction == 2 && s->direction != 0) {
		s->direction = 2;
	}
	else if (direction == 3 && s->direction != 1) {
		s->direction = 3;
	}
	return updateSnakePosition(s, eat);
}

int updateSnakePosition(snake* s, char eat){
	
	if (!eat) {
		s->shadow[0] = s->body[s->length * 2 - 2];
		s->shadow[1] = s->body[s->length * 2 - 1];
	}
	else{
		s->length++;
	}
	
	
	unsigned int i;
	for (i = (s->length * 2 - 1); i > 1; i -= 2) {
		s->body[i] = s->body[i-2];
		s->body[i-1] = s->body[i-3];
	}
	
	// Collision in walls
	if (s->direction == 0) {
		if (s->body[0] == 19) {
			return SNAKE_GAME_OVER;
		}
		s->body[0] = s->body[0] + 1 ;
	}
	else if (s->direction == 1) {
		if (s->body[1] == 14) {
			return SNAKE_GAME_OVER;
		}
		s->body[1]++;
	}
	else if (s->direction == 2) {
		if (s->body[0] == 0) {
			return SNAKE_GAME_OVER;
		}
		s->body[0]--;
	}
	else if (s->direction == 3) {
		if (s->body[1] == 0) {
			return SNAKE_GAME_OVER;
		}
		s->body[1]--;
	}
	
	// Collision in self
	
	for (i = 1; i < s->length; i++){
		if (s->body[0] == s->body[i*2] && s->body[1] == s->body[i*2 +1]) {
			return SNAKE_GAME_OVER;
		}
	}
	
	return SNAKE_MOVE_OK;
}

