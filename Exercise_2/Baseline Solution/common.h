#ifndef common
#define common

#define   SAMPLE_PERIOD 317

/*
 *	Function Declaration
 */
//========== Songs.c ==========//
void playNote(int frequency, int noteLength);
void playSong(int *notes, int *noteLengths, int noteCount);
int *instantiateSong(int songIndex);

//========== DAC.c ==========//
void setupDAC();
void writeToDAC(int sample);

//========== gpio.c ==========//
void setupGPIO();
int readButtons();

//========== timer.c ==========//
void setupTimer();


/*
 *	Macro declarations
 */
#define CHECK_BTN(input,btn) (~(input) & (1<<(btn)))	// Used to see if a button is pressed (active low)


/*
 *	Songs
 */
int happy[31];
int acid1[20];
int acid2[24];
int explosion[48];

int *happySamples;
int *acid1Samples;
int *acid2Samples;
int *explosionSamples;


#endif
