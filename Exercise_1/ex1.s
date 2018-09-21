        .syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
    // Exception vector table
    // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
    // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset:
	
	LDR R1, =CMU_BASE					//Loads the adress of the CMU_BASE
	LDR R2, [R1, #CMU_HFPERCLKEN0]		//Loads the content of the CMU_HFPERCLKNO memory
	
	MOV R3, #1							//
	LSL R3, R3, #13						//Sets the 13th bit high (GPIO-bit)
	ORR R2, R2, R3						//Enables the 13th bit in addition to other bits enabled
	
	STR R2, [R1, #CMU_HFPERCLKEN0]		//Stores the new enable-flags in the CMUHFPERCLKEN0-memory slot
	
	//=============================/
	
	LDR R0, =GPIO_PA_BASE				//Sets drive strength
	MOV R2, #0x2						//
	STR R2, [R1]						//

	ADD R7, R0, #GPIO_DOUT				//GPIO_PA_DOUT Constant for writing LEDs

	ADD R1, R0, #GPIO_MODEH				//Sets Port A to output
	LDR R2, =0x55555555					//
	STR R2, [R1]						//
	
	LDR R0, =GPIO_PC_BASE				//Sets Port C to input-mode
	LDR R2, =0x33333333
	ADD R1, R0, #GPIO_MODEL				
	STR R2, [R1]
	
	ADD R8, R0, #GPIO_DIN				//GPIO_PC_DIN Constant for reading buttons
	
	MOV R2, #0xff						//Enables pull-up resistors (C-port)
	ADD R1, R0, #GPIO_DOUT
	STR R2, [R1]
	
	
	
	/*
	ADD R1, R0, #GPIO_DOUT		
		//Led-test
	MOV R2, 0x1A						//
	LSL R2, #8							//
	STR R2, [R1]						//
	*/
	
	
	MOV R1, #0x1A
	LSL R1, #8
	STR R1, [R7]
	
	
		.thumb_func
polling_func:
	
	LDR R0, [R8]						//Button state
	AND R2, R0, #0x20
	CMP R2, #0x0
	BEQ add_dot
	AND R2, R0, #0x80
	CMP R2, #0x0
	BEQ remove_dot
	
	
	B polling_func

add_dot:
	LSR R1, #8
	MOV R2, #0x10
	
	EOR R1, R1, #0xff
	
	ORR R1, R1, R2
	EOR R1, R1, #0xff
	LSL R1, #8
	STR R1, [R7]
	
	B polling_func

remove_dot:

	LSR R1, #8
	MOV R2, #0x10
	
	ORR R1, R1, R2
	
	LSL R1, #8
	STR R1, [R7]

	B polling_func	

	/////////////////////////////////////////////////////////////////////////////
	//
    // GPIO handler
    // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////

	
        .thumb_func
gpio_handler:  

	      b .  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	
	.thumb_func
dummy_handler:  
        b .  // do nothing	

