#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "common.h"


/*
 *	Handles the input from buttons and plays song if associated button is pressed
 */
void handleInput(int buttonValues)
{
	if (CHECK_BTN(buttonValues, 4)) {	// If button 5 is pressed
		playSong(happy, happySamples, sizeof(happy)/sizeof(int));
	}
	else if (CHECK_BTN(buttonValues, 5)) {
		playSong(acid1, acid1Samples, sizeof(acid1)/sizeof(int));
	}
	else if (CHECK_BTN(buttonValues, 6)) {
		playSong(acid2, acid2Samples, sizeof(acid2)/sizeof(int));
	}
	else if (CHECK_BTN(buttonValues, 7)) {
		playSong(explosion, explosionSamples, sizeof(explosion)/sizeof(int));
	}
}

int main(void)
{
	// Setup functions
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	
	happySamples = instantiateSong(0);
	acid1Samples = instantiateSong(1);
	acid2Samples = instantiateSong(2);
	explosionSamples = instantiateSong(3);
	
	// Polling loop
	while(1) { 
		int buttonValues = readButtons();
		handleInput(buttonValues);
	}
	
	return 0;
}
