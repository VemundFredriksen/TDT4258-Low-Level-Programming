#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "efm32gg.h"
#include "math.h"
#include "songs.c"
#include "tones.h"

/*
 * The period between sound samples, in clock cycles 
 * 317 as sample period results in a sample rate of approximately 44100Hz
 */
#define   SAMPLE_PERIOD 317
#define CHECK_BTN(input,btn) (~(input) & (1<<(btn)))		//Used to see if a button is pressed (active low)

/*
*	Songs
*/
int happyBirthday[] = { C4, R, C4, D4, D4, D4, C4, C4, C4, F4, F4, F4, E4, E4, E4, E4, E4, R, C4, R, C4, D4, D4, D4, C4, C4, C4, G4, G4, G4, F4, F4, F4, F4, F4, R, C4, R, C4, C5, C5, C5, A4, A4, A4, F4, R, F4, E4, E4, E4, D4, D4, D4, B4, R, B4, A4, A4, A4, F4, F4, F4, G4, G4, G4, F4, F4, F4, F4 };

int acidSong[] = {C6, C6, R, A6, A6, R, A6, A6, R, C6, C6, E5};
int lostLife[] = {E4, F4, Fs4, G4, A4, G4, F4, Ds4};
int temp[] = {C5, C5, C5, C5, C5, G4, G4, G4, G4, E4, E4, E4, E4, C4, C4, C4, C4, C4 };

/*
 * Declaration of peripheral setup functions 
 */
void setupTimer(uint32_t period);
void setupGPIO();
void setupDAC();
int readButtons();
void handleInput();


/*
 * Your code will start executing here 
 */
int main(void)
{
	/*
	 * Setup functions
	 */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	
	//Polling loop
	while(1){ 
		int buttonValues = readButtons();
		handleInput(buttonValues);
	}
	
	return 0;
}

/*
*	Handles the input from buttons and plays song if associated button is pressed
*
*/
void handleInput(int buttonValues)
{
	if(CHECK_BTN(buttonValues, 5)){ //If button 6 is pressed
		playSong(happyBirthday, sizeof(happyBirthday)/sizeof(int));
	}
	else if(CHECK_BTN(buttonValues, 6)){
		playSong(acidSong, sizeof(acidSong)/sizeof(int));
	}
	else if(CHECK_BTN(buttonValues, 7)){
		playSong(lostLife, sizeof(lostLife)/sizeof(int));
	}
	else if(CHECK_BTN(buttonValues, 4)){
		playSong(temp, sizeof(temp)/sizeof(int));
	}
}
