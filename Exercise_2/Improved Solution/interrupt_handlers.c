#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "common.h"	

//========== Local Function declaration ==========//
void handleInput();
void setSong(int appliedSong[], int lenghtOfNotes[], int lengthOfSong);
void onEndedSong();

int interruptCount = 0;		//Counts the number of interrupts
int amplitude = 70;			//The maximum amplitude
int note = 0;				//Keeps track of the note position in a song/sound
int *song;					//A pointer to the song currently being played
int *noteLengths;			//How long each note should last
int songLength;				//Number of notes in a song/sound

/*
 * TIMER1 interrupt handler 
 */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	*TIMER1_IFC = 1;						//Clears interruptflag
	interruptCount++;
	
	if(interruptCount > noteLengths[note]){	//When to increment to next note
		note++;
		interruptCount = 0;
	}
	
	if (note >= songLength) {				//When all notes are played
		onEndedSong();
		return;
	}
	
	if (interruptCount % (22100/song[note]) == 0) {
		amplitude = (amplitude == 70) ? 0 : 70;
		writeToDAC(amplitude);
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

//Handles the interrupts from the controller
void handleInput()
{
	*GPIO_IFC = *GPIO_IF;				//Clears interruptflag 
	toSleep(2);						//Enters energy mode 2 (sleep on exit, not deepsleep)
	int buttonValues = *GPIO_PC_DIN;
	
	if (CHECK_BTN(buttonValues, 4)) { // If button 5 is pressed
/*		int* happySamples = instantiateSong(0);*/
		*GPIO_PA_DOUT = ~(1 << 12);
		setSong(happy, happySamples, sizeof(happy)/sizeof(int));
	}
	else if (CHECK_BTN(buttonValues, 5)) {
/*		int* acid1Samples = instantiateSong(1);*/
		*GPIO_PA_DOUT = ~(1 << 13);
		setSong(acid1, acid1Samples, sizeof(acid1)/sizeof(int));
	}
	else if (CHECK_BTN(buttonValues, 6)) {
/*		int* acid2Samples = instantiateSong(2);*/
		*GPIO_PA_DOUT = ~(1 << 14);
		setSong(acid2, acid2Samples, sizeof(acid2)/sizeof(int));
	}
	else if (CHECK_BTN(buttonValues, 7)) {
/*		int* explosionSamples = instantiateSong(3);*/
		*GPIO_PA_DOUT = ~(1 << 15);
		setSong(explosion, explosionSamples, sizeof(explosion)/sizeof(int));
	}
	else {
		toSleep(6);
	}
}

void setSong(int appliedSong[], int lengthOfNotes[], int lengthOfSong)
{
	note = 0;
	song = appliedSong;
	noteLengths = lengthOfNotes;
	songLength = lengthOfSong;
	setupDAC();
	startTimer();
}

void onEndedSong()
{
	stopTimer();				
	deactivateDAC();
	*GPIO_PA_DOUT = 0xFF << 8; 	//Clears the LEDlights
	toSleep(6);
}

