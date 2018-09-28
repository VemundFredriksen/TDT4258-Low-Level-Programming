# Example Makefile
#
# Exercise 1, TDT4258

LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=efm32gg.ld


ex1_pol.bin : ex1_pol.elf
	${OBJCOPY} -j .text -O binary $< $@

ex1_pol.elf : ex1_pol.o 
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS}

ex1_pol.o : ex1_pol.s
	${AS} ${ASFLAGS} $< -o $@


.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex1_pol.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex
