ifeq ($(CONFIG_FLASH_MEM_BASE),0x00000000)
obj-y += bootloader.o
endif

ifeq ($(CONFIG_USE_OF),y)
$(obj)/bootloader.o: $(obj)/../boot/dts/efm32gg-dk3750.dtb
endif

obj-y += common.o dtmachine.o

obj-$(CONFIG_PM) += pm.o
