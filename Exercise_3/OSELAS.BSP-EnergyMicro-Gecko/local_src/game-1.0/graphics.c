#include "graphics.h"

int screenDriver;
unsigned short* screenBuffer;
struct fb_copyarea area;

void graphicsInit()
{
	screenDriver = open("/dev/fb0", 2);
	screenBuffer = (unsigned short*) mmap(NULL, 2*320*240, PROT_WRITE | PROT_READ, MAP_SHARED, screenDriver, 0);
	
	area.width = 16;
	area.height = 16;
}

void writeToScreen(unsigned short val, unsigned int x, unsigned int y)
{

	unsigned int a;
	unsigned int b;
	
	for(a = y * 16; a < (y*16 + 16); a++){
		for(b = x * 16; b < (x*16 + 16); b++){
			screenBuffer[a * 320 + b] = val;
		}
	}

	area.dx = x * 16;
	area.dy = y * 16;
	
	ioctl(screenDriver, 0x4680, &area);
}

void clearScreen()
{
	area.dx = 0;
	area.dy = 0;
	area.width = 320;
	area.height = 240;
	
	unsigned int i;
	for(i = 0; i < 2*38400; i++){
		screenBuffer[i] = 0x0000;
	}
	
	ioctl(screenDriver, 0x4680, &area);
	area.width = 16;
	area.height = 16;
}

