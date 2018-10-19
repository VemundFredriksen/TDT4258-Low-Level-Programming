#include <stdint.h>
#include <stdbool.h>
#include "songs.h"
#include "efm32gg.h"
#include "tones.h"
#include "timer.h"

#define CHECK_BTN(input,btn) (~(input) & (1<<(btn)))		//Used to see if a button is pressed (active low)

void handleInput();
//void playSong(int* notes, int noteCount, int noteLength);

int interruptCount = 0;
int amplitude = 70;
int note = 0;
int song[] = { C4, R, C4, D4, D4, D4, C4, C4, C4, F4, F4, F4, E4, E4, E4, E4, E4, R, C4, R, C4, D4, D4, D4, C4, C4, C4, G4, G4, G4, F4, F4, F4, F4, F4, R, C4, R, C4, C5, C5, C5, A4, A4, A4, F4, R, F4, E4, E4, E4, D4, D4, D4, B4, R, B4, A4, A4, A4, F4, F4, F4, G4, G4, G4, F4, F4, F4, F4 };
int noteLength = 5000;
int songLength = sizeof(song)/sizeof(int);
/*
 * TIMER1 interrupt handler 
 */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	*TIMER1_IFC = 1;
	/*
	 * TODO feed new samples to the DAC remember to clear the pending
	 * interrupt by writing 1 to TIMER1_IFC 
	 */
	interruptCount++;
	
	if(interruptCount > noteLength){
		note++;
		interruptCount = 0;
	}
	if(note >= songLength){
		stopTimer();
		note = 0;
		*DAC0_CH0DATA = 0;
		*DAC0_CH1DATA = 0;
		return;
	}
	
	if(interruptCount % (22100/song[note]) == 0){
	
		if(amplitude == 70){
			amplitude = 0;
		}
		else{
		
			amplitude = 70;
		}
		*DAC0_CH0DATA = amplitude;
		*DAC0_CH1DATA = amplitude;
	}
	
	 
}

/*
 * GPIO even pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	*GPIO_IFC = *GPIO_IF;
	/*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
	 handleInput();
	 
}

/*
 * GPIO odd pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	*GPIO_IFC = *GPIO_IF;
	/*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
	 handleInput();
}

int happyBirthday[] = { C4, R, C4, D4, D4, D4, C4, C4, C4, F4, F4, F4, E4, E4, E4, E4, E4, R, C4, R, C4, D4, D4, D4, C4, C4, C4, G4, G4, G4, F4, F4, F4, F4, F4, R, C4, R, C4, C5, C5, C5, A4, A4, A4, F4, R, F4, E4, E4, E4, D4, D4, D4, B4, R, B4, A4, A4, A4, F4, F4, F4, G4, G4, G4, F4, F4, F4, F4 };

void handleInput()
{
	int buttonValues = *GPIO_PC_DIN;
	if(CHECK_BTN(buttonValues, 5)){ //If button 6 is pressed
		*GPIO_PA_DOUT = buttonValues << 8;
		playSong(happyBirthday, sizeof(happyBirthday)/sizeof(int), 1000);
	}
	else if(CHECK_BTN(buttonValues, 6)){
		//playSong(acidSong, sizeof(acidSong)/sizeof(int), 1000);
		startTimer();
	}
	else if(CHECK_BTN(buttonValues, 7)){
		//playSong(lostLife, sizeof(lostLife)/sizeof(int), 1000);
	}
	else if(CHECK_BTN(buttonValues, 4)){
		//playSong(temp, sizeof(temp)/sizeof(int), 1000);
	}
}

