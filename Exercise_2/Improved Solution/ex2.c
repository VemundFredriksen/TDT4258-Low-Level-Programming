#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "common.h"

/*
 *	Local Function Declarations
 */
void setupGPIO();
void setupNVIC();

int main(void)
{
	setupGPIO();
	setupTimer(SAMPLE_PERIOD);
	setupNVIC();
	setupDeepSleep();
	
	//Enter deep sleep (Wait For Interrupt)
	__asm__("WFI");
	
	return 0;
}

//Enable interrupts
void setupNVIC()
{	 
	 *ISER0 = 0x1802;
}

//Enables energymode = deepsleep
void setupDeepSleep()
{
	*SCR = 6;
}
