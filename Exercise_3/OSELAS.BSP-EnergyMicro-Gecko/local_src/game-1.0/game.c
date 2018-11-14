#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>

#include "graphics.h"
#include "snake.h"

#define TICK_RATE 8

void input_handler(int sig);
void game_tick();
int setup_gamepad();
void setup_timer();

struct sigaction sa;
struct itimerval timer;

snake sMan;
unsigned char reqDirection = 0;

FILE* device;

//TODO always feedSnake before requesting move

unsigned short gameBoard[15*20];
unsigned short food[2];

int main(int argc, char *argv[])
{
	printf("Welcome to Travelling Snakesman!\n");
	int result = setup_gamepad();
	graphicsInit();
	setup_timer();
	srand(time(NULL));
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
	
	if(input == 191){
		reqDirection = 0;
	}
	else if(input == 127){
		reqDirection = 1;
	}
	else if(input == 239){
		reqDirection = 2;
	}
	else if(input == 223){
		reqDirection = 3;
	}
}

void restartGame(){
	food[0] = 12;
	food[1] = 10;
	reqDirection = 0;
	clearGameBoard();
	clearScreen();
	setupSnake();
	updateGameBoard();
	updateGraphics();
	
	
	
	
}

void game_tick(int signum)
{
	int snakeResult = SNAKE_MOVE_OK;
	char eat = 0;
	if(sMan.body[0] == food[0] && sMan.body[1] == food[1]){
		eat = 1;
		updateFood();
	}
	snakeResult = makeMove(&sMan, reqDirection, eat);
	if(snakeResult == SNAKE_GAME_OVER){
		restartGame();
	}
	updateGameBoard(eat);
	updateGraphics(eat);
}

void updateGameBoard(char eaten)
{
	gameBoard[sMan.shadow[1] * 20 + sMan.shadow[0]] = 0;
	if(eaten){
		gameBoard[food[1] * 20 + food[0]] = 3;
	}
	
	gameBoard[sMan.body[1] * 20 + sMan.body[0]] = 2;
	gameBoard[sMan.body[3] *20 + sMan.body[2]] = 1;
}

void updateGraphics(char eaten)
{
	writeToScreen(SNAKE_BG, sMan.shadow[0], sMan.shadow[1]);
	if(eaten){
		writeToScreen(SNAKE_FOOD, food[0], food[1]);
	}

	writeToScreen(SNAKE_HEAD, sMan.body[0], sMan.body[1]);
	writeToScreen(SNAKE_BODY, sMan.body[2], sMan.body[3]);
}

void updateFood()
{
	unsigned short nextPos = 1 + rand() % (299 - sMan.length);
	unsigned short i = 0;
	
	while(nextPos > 0){
		if(!gameBoard[i]){
			nextPos--;
		}
		i++;
	}
	
	food[0] = (i-1) % 20;
	food[1] = (unsigned short) (i-1) / 20;
	
}

void setupSnake(){
	sMan = snakeInit();
}

void clearGameBoard()
{
	unsigned int i;
	for(i = 0; i< 300; i++){
		gameBoard[i] = 0;
	}
}

void setup_timer()
{
	 /* Install timer_handler as the signal handler for SIGVTALRM. */
	 memset (&sa, 0, sizeof (sa));
	 sa.sa_handler = &game_tick;
	 sigaction (SIGVTALRM, &sa, NULL);

	 timer.it_value.tv_sec = 0;
	 timer.it_value.tv_usec = 500000;

	 timer.it_interval.tv_sec = 0;
	 timer.it_interval.tv_usec = 1000000 / TICK_RATE;
	 /* Start a virtual timer. It counts down whenever this process is
	   executing. */
	 setitimer (ITIMER_VIRTUAL, &timer, NULL);
}
