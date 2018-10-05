#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * function to setup the timer 
 */
void setupTimer(uint16_t period)
{
	/*
	 * TODO enable and set up the timer
	 * 
	 * 1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0 2.
	 * Write the period to register TIMER1_TOP 3. Enable timer interrupt
	 * generation by writing 1 to TIMER1_IEN 4. Start the timer by writing 
	 * 1 to TIMER1_CMD
	 * 
	 * This will cause a timer interrupt to be generated every (period)
	 * cycles. Remember to configure the NVIC as well, otherwise the
	 * interrupt handler will not be invoked. 
	 */
	 
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
