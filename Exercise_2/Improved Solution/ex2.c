#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "common.h"


// Enable interrupts
void setupNVIC()
{
	*ISER0 = 0x1802;
}

// Enable given energyMode
void toSleep(int energyMode)
{
	*SCR = energyMode;
}

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

	// Enter deep sleep (Wait For Interrupt)
	while (1) {
		__asm__("WFI");
	}

	return 0;
}
