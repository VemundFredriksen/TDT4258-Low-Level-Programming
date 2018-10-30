#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "common.h"

//========== Local Function declaration ==========//
void handleInput();
void setSong(int appliedSong[], int lenghtOfNotes[], int lengthOfSong);
void onEndedSong();

int interruptCount = 0;
int sample = 70;
int noteIndex = 0;
int *notes;
int *noteLengths;
int noteCount;

/*
 * TIMER1 interrupt handler 
 */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	*TIMER1_IFC = 1;	// Clear interrupt flag
	interruptCount++;
	
	// When to increment to next note
	if (interruptCount > noteLengths[noteIndex]) {
		noteIndex++;
		interruptCount = 0;
	}
	
	if (noteIndex >= noteCount) {	// When all notes are played
		onEndedSong();
		return;
	}
	
	if (interruptCount % (22100 / notes[noteIndex]) == 0) {
		sample = (sample == 70) ? 0 : 70;
		writeToDAC(sample);
	}
}

/*
 * GPIO even pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	handleInput();
}

/*
 * GPIO odd pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	handleInput();
}

/*
 * GPIO interrupt handler 
 */
void handleInput()
{
	*GPIO_IFC = *GPIO_IF;	// Clear interrupt flag 
	toSleep(2);
	int buttonValues = readButtons();

	if (CHECK_BTN(buttonValues, 4)) {	// If button SW5 is pressed
		*GPIO_PA_DOUT = ~(1 << 12);
		setSong(happy, happySamples, sizeof(happy) / sizeof(int));
	} else if (CHECK_BTN(buttonValues, 5)) {
		*GPIO_PA_DOUT = ~(1 << 13);
		setSong(acid1, acid1Samples, sizeof(acid1) / sizeof(int));
	} else if (CHECK_BTN(buttonValues, 6)) {
		*GPIO_PA_DOUT = ~(1 << 14);
		setSong(acid2, acid2Samples, sizeof(acid2) / sizeof(int));
	} else if (CHECK_BTN(buttonValues, 7)) {
		*GPIO_PA_DOUT = ~(1 << 15);
		setSong(explosion, explosionSamples,
			sizeof(explosion) / sizeof(int));
	} else {
		toSleep(6);
	}
}

void setSong(int notesIn[], int noteLengthsIn[], int noteCountIn)
{
	noteIndex = 0;
	notes = notesIn;
	noteLengths = noteLengthsIn;
	noteCount = noteCountIn;
	activateDAC();
	startTimer();
}

void onEndedSong()
{
	stopTimer();
	deactivateDAC();
	*GPIO_PA_DOUT = 0xFF << 8;	// Clear the LEDlights
	toSleep(6);
}
