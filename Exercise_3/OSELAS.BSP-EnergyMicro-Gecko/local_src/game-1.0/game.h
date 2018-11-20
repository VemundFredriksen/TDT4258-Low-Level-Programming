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
