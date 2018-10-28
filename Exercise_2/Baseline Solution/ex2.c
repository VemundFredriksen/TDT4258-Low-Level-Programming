#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "common.h"


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
		playSong(explosion, sizeof(explosion)/sizeof(int), 2500);
	}
	else if(CHECK_BTN(buttonValues, 6)){
		playSong(happyBirthday, sizeof(happyBirthday)/sizeof(int), 1000);
	}
	else if(CHECK_BTN(buttonValues, 7)){
		playSong(acidSong, sizeof(acidSong)/sizeof(int), 1000);
	}
	else if(CHECK_BTN(buttonValues, 4)){
		playSong(acidSound, sizeof(acidSound)/sizeof(int), 1000);
	}
}
