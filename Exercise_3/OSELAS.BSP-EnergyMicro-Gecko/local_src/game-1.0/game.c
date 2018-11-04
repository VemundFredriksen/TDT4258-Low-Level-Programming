#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>

FILE* device;

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	device = fopen("/dev/gamepad", "rb");
	int fbfd = open("/dev/fb0", 2);
	
	unsigned short* screen;
	screen = (unsigned short*) mmap(NULL, 2*320*240, PROT_WRITE | PROT_READ, MAP_SHARED, fbfd, 0);
	int i;
	
	for(i = 0; i < 2*38400; i++){
		screen[i] = 0x001F;
	}
	
	struct fb_copyarea rect;
	
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;
	
	
	ioctl(fbfd, 0x4680, &rect);
	exit(EXIT_SUCCESS);
}
