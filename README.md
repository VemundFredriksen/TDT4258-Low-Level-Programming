# TDT4258 - Low Level Programming

The course focuses on low level programming of embedded systems in assembly and C. The course also focuses on power saving techniques. In the exercises the EFM32 Giant Gecko is used.

## Exercise 1
**Assembly, Memory Mapped GPIO, Power Saving, Interrupt Handling, Deep Sleep**

In exercise 1 the group programmed the gecko to utilize a controller connected to the processor through the General Purpose Input Output (GPIO) ports. The processor reads the input from the buttons through the memory mapped GPIO interface and lights up LEDs on the same controller through the GPIO interface. In the exercise there are produced two different versions of the program where the improved version focuses on power saving. Both solutions are written entirely in assembly for ARM.

## Exercise 2
**C, DAC**

Exercise 2 is mainly about writing readable and effective code in the programming language C. The group programmed the gecko to take input from an external controller and play music or sounds on the geckos DAC. Like in exercise 1 the GPIO is memory mapped, there is focus on writing energy efficient code and both a baseline, busy-waiting version as well as an improved version using interrupts and deep sleeps are produced. 
