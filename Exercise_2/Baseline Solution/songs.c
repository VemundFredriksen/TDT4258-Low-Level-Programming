#include "efm32gg.h"
#include "common.h"
#include "tones.h"

void playSong(int* notes, int noteCount, int noteLength){	
	
	//Transforms the tones from Hz to number of interrupts to pass before writing new value
	int interruptFreq[noteCount];
	for(int i = 0; i < noteCount; i++){
		interruptFreq[i] = (10000/notes[i]);
	}
	
	//Plays through the whole song
	for(int i = 0; i < noteCount; i++){
		playNote(interruptFreq[i], noteLength);
	}
	
	*DAC0_CH0DATA = 0;
	*DAC0_CH1DATA = 0;
	
}

void playNote(int frequency, int noteLength){
	int clockVal = *TIMER1_CNT;						//Keeps track of the timer count
	int roundVal = 1;								//Keeps track of number of interrupts
	int sample = 0;
	while(roundVal < noteLength){
		if(*TIMER1_CNT < clockVal){					//If the new timercount is less than the previous, then "a interrupt is triggered"
			*TIMER1_CMD = 2;						//Stops the timer while doing the most tidious work in the loop
			if(roundVal % frequency == 0){			//If the roundval corresponds to the right number of interrupts associtated with the tone being played
													//Then change the DAC value
				if(sample == 70){
					sample = 0;
				} else {
					sample= 70;
				}
			}
			roundVal++;								
			*DAC0_CH0DATA = sample;
			*DAC0_CH1DATA = sample;
			*TIMER1_CMD = 1;						//Start the timer count before entering the loop again
		}
		clockVal = *TIMER1_CNT;						//Continuously read the timer count
	}
}

/*
 *	Songs
 */
int happyBirthday[] = { C4, R, C4, D4, D4, D4, C4, C4, C4, F4, F4, F4, E4, E4, E4, E4, E4, R, C4, R, C4, D4, D4, D4, C4, C4, C4, G4, G4, G4, F4, F4, F4, F4, F4, R, C4, R, C4, C5, C5, C5, A4, A4, A4, F4, R, F4, E4, E4, E4, D4, D4, D4, B4, R, B4, A4, A4, A4, F4, F4, F4, G4, G4, G4, F4, F4, F4, F4 };

int acidSong[] = {C4, R, C4, D4, D4,C4, R, C4, D4, D4,C4, R, C4, D4, D4,C4, R, C4, D4, D4};

int acidSound[] = {G4, B4, G5, B5, G6, G6, Fs5, C5,G4, B4, G5, B5, G6, G6, Fs5, C5,G4, B4, G5, B5, G6, G6, Fs5, C5};

int explosion[] = {G4/12, B4/12, G5/12, B5/12, G6/12, G6/12, Fs5/12, C5/12,G4/12, B4/6, G5/6, B5/6, G6/4, G6/4, Fs5/4, C5/4,G4/4, B4/4, G5/4, B5/4, G6/4, G6*7, Fs5/4, C5/4,G4*2, B4*4, G5/12, B5/12, G6/12, G6/12, Fs5/12, C5/12,G4/12, B4/6, G5/6, B5/6, G6/4, G6/4, Fs5/4, C5/4,G4/4, B4/4, G5/4, B5*5, G6/4, G6/4, Fs5/4, C5/4};
