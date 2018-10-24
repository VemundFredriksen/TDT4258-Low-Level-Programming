#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "efm32gg.h"

/*
 * function to setup the timer 
 */
void setupTimer(uint16_t period)
{

	 // This will cause a timer interrupt to be generated every (period)
	 // cycles. Remember to configure the NVIC as well, otherwise the
	 // interrupt handler will not be invoked. 
	 
	 
	 //Enable timer clock
	 uint32_t timerEnable = 1 << 6;
	 *CMU_HFPERCLKEN0 |= timerEnable;
	 
	 //Writes the period to the timer
	 *TIMER1_TOP = period;
	 
	 //Enable timer interrupt generation
	 *TIMER1_IEN = 1;
}

inline void startTimer()
{
	 *CMU_HFPERCLKEN0 |= (1 << 6);
	 *TIMER1_IEN = 1;
	 *TIMER1_CMD = 1;
}

inline void stopTimer()
{
	 *CMU_HFPERCLKEN0 &= ~(1 << 6);
	 *TIMER1_IEN = 0;
	 *TIMER1_CMD = 2;
}

