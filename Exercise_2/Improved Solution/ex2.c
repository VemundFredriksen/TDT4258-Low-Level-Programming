#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "common.h"

/*
 *	Local Function Declarations
 */
void setupGPIO();
void setupNVIC();
int *instantiateSong(int songIndex);

int* happySamples;
int* acid1Samples;
int* acid2Samples;
int* explosionSamples;

int main(void)
{
	setupGPIO();
	setupTimer(SAMPLE_PERIOD);
	setupNVIC();
	toSleep(6);
	
	happySamples = instantiateSong(0);
	acid1Samples = instantiateSong(1);
	acid2Samples = instantiateSong(2);
	explosionSamples = instantiateSong(3);
	
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
void toSleep(int energyMode)
{
	*SCR = energyMode;
}
