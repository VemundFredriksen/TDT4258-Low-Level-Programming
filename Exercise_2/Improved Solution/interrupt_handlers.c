#include <stdint.h>
#include <stdbool.h>
#include "songs.h"
#include "efm32gg.h"
#include "timer.h"
#include "dac.h"

//Returns true if given button is pressed
#define CHECK_BTN(input,btn) (~(input) & (1<<(btn)))		

void handleInput();

int interruptCount = 0;		//Counts the number of interrupts
int amplitude = 70;			//The maximum amplitude
int note = 0;				//Keeps track of the note position in a song/sound
int *song;					//A pointer to the song currently being played
int noteLength;				//How long each note should last
int songLength;				//Number of notes in a song/sound

/*
 * TIMER1 interrupt handler 
 */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	*TIMER1_IFC = 1;
	interruptCount++;
	
	if(interruptCount > noteLength){
		note++;
		interruptCount = 0;
	}
	if(note >= songLength){
		onEndedSong();
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
	*GPIO_IFC = *GPIO_IF;
	int buttonValues = *GPIO_PC_DIN;
	
	if(CHECK_BTN(buttonValues, 5)){ //If button 6 is pressed
		*GPIO_PA_DOUT = ~(1 << 13);
		setSong(explosion, sizeof(explosion)/sizeof(int), 5000);
	}
	else if(CHECK_BTN(buttonValues, 6)){
		*GPIO_PA_DOUT = ~(1 << 14);
		setSong(happyBirthday, sizeof(happyBirthday)/sizeof(int), 5000);
		
	}
	else if(CHECK_BTN(buttonValues, 7)){
		*GPIO_PA_DOUT = ~(1 << 15);
		setSong(acidSong, sizeof(acidSong)/sizeof(int), 5000);
	}
	else if(CHECK_BTN(buttonValues, 4)){
		*GPIO_PA_DOUT = ~(1 << 12);
		setSong(acidSound, sizeof(acidSound)/sizeof(int), 500);
	}
}

void setSong(int appliedSong[], int lengthOfSong, int lengthOfNotes)
{
		*SCR = 2;
		note = 0;
		song = appliedSong;
		noteLength = lengthOfNotes;
		songLength = lengthOfSong;
		setupDAC();
		startTimer();
}

void onEndedSong()
{
		stopTimer();
		deactivateDAC();
		*GPIO_PA_DOUT = 0xFF << 8; //Clears the LEDlights
		note = 0;
		*DAC0_CH0DATA = 0;
		*DAC0_CH1DATA = 0;
		*SCR = 6;
}

