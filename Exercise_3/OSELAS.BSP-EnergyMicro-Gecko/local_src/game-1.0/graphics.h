

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>

void graphicsInit();
void writeToScreen(unsigned short val, unsigned int x, unsigned int y);
void clearScreen();

#endif
