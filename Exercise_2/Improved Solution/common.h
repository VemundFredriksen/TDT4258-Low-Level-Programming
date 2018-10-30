#ifndef common
#define common

// The period between sound samples, in clock cycles 
#define   SAMPLE_PERIOD   317

//========== Song declarations ==========//
int happy[31];
int acid1[20];
int acid2[24];
int explosion[48];

int *happySamples;
int *acid1Samples;
int *acid2Samples;
int *explosionSamples;


/*
 *	Function declarations
 */

//========== dac.c ==========//
void activateDAC();
void deactivateDAC();
void writeToDAC(int amplitude);

//========== timer.c ==========//
void setupTimer(uint16_t period);
void startTimer();
void stopTimer();

//========== ex2.c ==========//
void toSleep(int energyMode);

//========== songs.c ==========//
int *instantiateSong(int songIndex);

//========== gpio.c ==========//
void setupGPIO();
int readButtons();


/*
 *	Macro declarations
 */

// Expects the buttoninput from GPIO and the button to check if this button is pressed
#define CHECK_BTN(input,btn) (~(input) & (1<<(btn)))

#endif
