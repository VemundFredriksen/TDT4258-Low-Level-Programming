#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

void input_handler(int sig);
int setup_gamepad();

FILE* device;

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	int result = setup_gamepad();
	printf("%d\n", result);
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
	while(1){
	
	}
	
	ioctl(fbfd, 0x4680, &rect);
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
	printf("inp: %d\n", input);
	
}
