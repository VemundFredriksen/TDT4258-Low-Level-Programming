#ifndef common
#define common

#define   SAMPLE_PERIOD 317

/*
 *	Function Declaration
 */

//========== Songs.c ==========//
void playNote(int frequency, int noteLength);
void playSong(int* notes, int noteCount, int noteLength);

//========== DAC.c ==========//
void setupDAC();

//========== Gpio.c ==========//
void setupGPIO();
int readButtons();

//========== timer.c ==========//
void setupTimer();

//========== ex2.c ==========//
void handleInput();


/*
 *	Macro declarations
 */
#define CHECK_BTN(input,btn) (~(input) & (1<<(btn)))		//Used to see if a button is pressed (active low)

/*
 *	Songs
 */
int happyBirthday[70];
int acidSong[20];
int acidSound[24];
int explosion[48];

#endif
