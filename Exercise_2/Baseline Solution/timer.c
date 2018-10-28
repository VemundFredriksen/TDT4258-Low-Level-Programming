#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"

/*
 * function to setup the timer 
 */
void setupTimer(uint16_t period)
{	 
	 //Enable timer clock
	 uint32_t timerEnable = 1 << 6;
	 *CMU_HFPERCLKEN0 |= timerEnable;
	 
	 //Writes the period to the timer
	 *TIMER1_TOP = period;
	 
	 //Enable timer interrupt generation
	 *TIMER1_IEN = 1;
	 
	 //Start the timer
	 *TIMER1_CMD = 1;
	 
}
