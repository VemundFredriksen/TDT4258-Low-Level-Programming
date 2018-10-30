#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"

void setupTimer(uint16_t period)
{
	// Enable timer clock
	uint32_t timerEnable = 1 << 6;
	*CMU_HFPERCLKEN0 |= timerEnable;

	// Writes the period to the timer
	*TIMER1_TOP = period;

	// Start the timer
	*TIMER1_CMD = 1;
}
