#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * function to set up GPIO mode and interrupts
 */
void setupGPIO()
{
	/*
	 * Enable GPIO clock
	 */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;
	
	/*
	 * Set up gamepad LED lights as output
	 */
	*GPIO_PA_CTRL = 2;	/* set high drive strength */
	*GPIO_PA_MODEH = 0x55555555;	/* set pins A8-15 as output */
	*GPIO_PA_DOUT = 0xFFFFFFFF;		//Turns the LED lights of
	
	/*
	 * Set up gamepad buttons as input
	 */
	*GPIO_PC_MODEL = 0x33333333;	/* set pins D0-7 as input */
	
	/*
	 * Enables pullup resistors on buttons
	 */
	 *GPIO_PC_DOUT = 0xFF;
	 
	  //============ ENABLE INTERRUPT ON GPIO ===========//
	  
	  //Clears interruptflag
	  *GPIO_IFC = *GPIO_IF;
	  
	  // -----
	  *GPIO_EXTIPSELL = 0x22222222;
	  *GPIO_IFC = *GPIO_IF;
	  
	  //Enables interrupt on falling edge
	  *GPIO_EXTIFALL = 0xFF;
	  *GPIO_IFC = *GPIO_IF;
	  
	  //Enables interrupt on GPIO
	  *GPIO_IEN = 0xFF;
	  *GPIO_IFC = *GPIO_IF;
	  
}

int readButtons()
{
	return *GPIO_PC_DIN;
}
