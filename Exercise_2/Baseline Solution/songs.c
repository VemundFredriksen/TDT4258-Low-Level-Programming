#include "efm32gg.h"

void playSong(int tempo, int* notes, int noteCount){
	
	int notePos = 0;
	tempo = 44100/(tempo/60);
	
	
	int transformed[noteCount];
	
	for(int i = 0; i < noteCount; i++){
		transformed[i] = (44100/notes[i]);
	}
	
	for(int i = 0; i < noteCount; i++){
		playNote(transformed[i]);
	}
	
	/*
	for(int i = 0; i < 500000; i++){
		*DAC0_CH0DATA = 0;
		*DAC0_CH1DATA = 0;
	}
	int tone = transformed[notePos];

	while (notePos < noteCount){
	
		if(*TIMER1_CNT < clockVal){
			if(roundVal % tempo == 0){
				roundVal = 1;
				notePos++;
			}
			if(roundVal % transformed[notePos] == 0){
				
				if(sample == 150){
					sample = 0;
				} else {
					sample= 150;
				}
				
			}
			roundVal++;
			*DAC0_CH0DATA = sample;
			*DAC0_CH1DATA = sample;
		}
		clockVal = *TIMER1_CNT;
	}
	
	for(int i = 0; i < noteCount; i++){
		playNote(transformed[i]);
	}
	
	int n = 0;
	playNote(transformed[0]);
	playNote(transformed[0]);
	playNote(transformed[noteCount - 1]);
	playNote(transformed[noteCount - 1]);
	playNote(transformed[0]);
	playNote(transformed[0]);
	*/
	*DAC0_CH0DATA = 0;
	*DAC0_CH1DATA = 0;
	
}

void playNote(int frequency){
	int clockVal = *TIMER1_CNT;
	int roundVal = 1;
	int sample = 0;
	while(roundVal < 44100){
		if(*TIMER1_CNT < clockVal){
			
			if(roundVal % frequency == 0){
				
				if(sample == 70){
					sample = 0;
				} else {
					sample= 70;
				}
			}
			roundVal++;
			*DAC0_CH0DATA = sample;
			*DAC0_CH1DATA = sample;
		}
		clockVal = *TIMER1_CNT;
	}
}
