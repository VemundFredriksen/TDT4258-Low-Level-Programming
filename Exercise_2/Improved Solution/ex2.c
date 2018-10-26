#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "timer.h"
#include "dac.h"


//The period between sound samples, in clock cycles 
#define   SAMPLE_PERIOD   317

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
