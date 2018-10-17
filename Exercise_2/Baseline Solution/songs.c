#include "efm32gg.h"

void playNote(int frequency);

void playSong(int* notes, int noteCount){
	
	
	//Transforms the tones from Hz to number of interrupts to pass before writing new value
	int interruptFreq[noteCount];
	for(int i = 0; i < noteCount; i++){
		interruptFreq[i] = (10000/notes[i]);
	}
	
	//Plays through the whole song
	for(int i = 0; i < noteCount; i++){
		playNote(interruptFreq[i]);
	}
	
	*DAC0_CH0DATA = 0;
	*DAC0_CH1DATA = 0;
	
}

void playNote(int frequency){
	int clockVal = *TIMER1_CNT;						//Keeps track of the timer count
	int roundVal = 1;								//Keeps track of number of interrupts
	int sample = 0;
	while(roundVal < 2100){
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
