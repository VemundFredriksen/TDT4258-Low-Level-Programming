#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include "efm32gg.h"


#define DRIVER_NAME "TDT4258_Gamepad_Driver"

int device_number;

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init setup(void)
{
	printk("Hello World, here is your module speaking\n");
	
	//int result;
	//result = alloc_chrdev_region(&device_number, 0, 1, DRIVER_NAME);
	// Allocating device numbers
	if(alloc_chrdev_region(&device_number, 0, 1, DRIVER_NAME) < 0){
		printk(KERN_ALERT "Error allocating driver numbers...\n");
		return -1;
	}
	
	// IO Port Allocation
	if(request_mem_region(GPIO_PC_MODEL, 4, DRIVER_NAME) == NULL){
		return -1;
	}
	
	if(request_mem_region(GPIO_PC_DIN, 4, DRIVER_NAME) == NULL){
		return -1;
	}
	
	if(request_mem_region(GPIO_PC_DOUT, 4, DRIVER_NAME) == NULL){
		return -1;
	}
	
	// Memory Mapping
	ioremap(GPIO_PC_MODEL, 4);
	ioremap(GPIO_PC_DIN, 4);
	
	//GPIO Button Setup
	iowrite32(0x33333333, GPIO_PC_MODEL);
	iowrite32(0xFF, GPIO_PC_DOUT);
	
	return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit teardown(void)
{
	 printk("Short life for a small module...\n");
}

module_init(setup);
module_exit(teardown);

MODULE_DESCRIPTION("TDT4258 Gamepad Driver");
MODULE_LICENSE("GPL");

