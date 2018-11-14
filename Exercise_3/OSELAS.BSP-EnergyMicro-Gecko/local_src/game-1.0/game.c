#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include "graphics.h"
#include "snake.h"

void input_handler(int sig);
int setup_gamepad();

snake sMan;

FILE* device;

//TODO always feedSnake before requesting move

unsigned short gameBoard[15*20];
unsigned short food[2];

int main(int argc, char *argv[])
{
	printf("Welcome to Travelling Snakesman!\n");
	int result = setup_gamepad();
	graphicsInit();
	restartGame();
	
	while(1){
	
	}
	exit(EXIT_SUCCESS);
}

int setup_gamepad()
{
	device = fopen("/dev/TDT4258_Gamepad_Driver", "rb");
    if (!device) {
        printf("Unable to open driver device, maybe you didn't load the module?\n");
        return EXIT_FAILURE;
    }
    if (signal(SIGIO, &input_handler) == SIG_ERR) {
        printf("An error occurred while register a signal handler.\n");
        return EXIT_FAILURE;
    }
    if (fcntl(fileno(device), F_SETOWN, getpid()) == -1) {
        printf("Error setting pid as owner.\n");
        return EXIT_FAILURE;
    }
    long oflags = fcntl(fileno(device), F_GETFL);
    if (fcntl(fileno(device), F_SETFL, oflags | FASYNC) == -1) {
        printf("Error setting FASYNC flag.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void input_handler(int sig)
{
	int input = fgetc(device);
	int snakeResult = SNAKE_MOVE_OK;
	char eat = 0;
	if(sMan.body[0] == food[0] && sMan.body[1] == food[1]){
		eat = 1;
		food[0] = food[0] -1;
		food[1]--;
	}
	if(input == 191){
		snakeResult = makeMove(&sMan, 0, eat);
	}
	else if(input == 127){
		snakeResult = makeMove(&sMan, 1, eat);
	}
	else if(input == 239){
		snakeResult = makeMove(&sMan, 2, eat);
	}
	else if(input == 223){
		snakeResult = makeMove(&sMan, 3, eat);
	}
	if(snakeResult == SNAKE_GAME_OVER){
		restartGame();
	}
	updateGraphics();
}

void restartGame(){
	clearScreen();
	setupSnake();
	
	food[0] = 12;
	food[1] = 10;
	
	
}

void updateGraphics()
{
	writeToScreen(SNAKE_HEAD, sMan.body[0], sMan.body[1]);
	writeToScreen(SNAKE_BODY, sMan.body[2], sMan.body[3]);
	writeToScreen(SNAKE_BG, sMan.shadow[0], sMan.shadow[1]);
	writeToScreen(SNAKE_FOOD, food[0], food[1]);
}

void updateFood()
{

}

void setupSnake(){
	sMan = snakeInit();
	
	writeToScreen(SNAKE_HEAD, sMan.body[0],sMan.body[1]);
	writeToScreen(SNAKE_BODY, sMan.body[2],sMan.body[3]);
	writeToScreen(SNAKE_BODY, sMan.body[4],sMan.body[5]);
}
