# Example Makefile
#
# Exercise 1, TDT4258

LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=efm32gg.ld


ex1_int.bin : ex1_int.elf
	${OBJCOPY} -j .text -O binary $< $@

ex1_int.elf : ex1_int.o 
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS}

ex1_int.o : ex1_int.s
	${AS} ${ASFLAGS} $< -o $@


.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex1_int.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex
