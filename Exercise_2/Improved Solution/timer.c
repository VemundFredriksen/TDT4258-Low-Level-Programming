#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"

void setupTimer(uint16_t period)
{
	// Enable timer clock
	*CMU_HFPERCLKEN0 |= (1 << 6);

	// Writes the period to the timer
	*TIMER1_TOP = period;
	
	// Disable timer clock
	*CMU_HFPERCLKEN0 &= ~(1 << 6);
}

void startTimer()
{
	*CMU_HFPERCLKEN0 |= (1 << 6);
	*TIMER1_IEN = 1;
	*TIMER1_CMD = 1;
}

void stopTimer()
{
	*CMU_HFPERCLKEN0 &= ~(1 << 6);
	*TIMER1_IEN = 0;
	*TIMER1_CMD = 2;
}
