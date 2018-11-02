/*
 * Register definition for efm32's CMU component
 */

#define CMU_OSCENCMD		0x20
#define CMU_OSCENCMD_HFXOEN		0x00000004

#define CMU_CMD			0x24
#define CMU_CMD_HFCLKSEL_HFXO		0x00000002

#define CMU_STATUS		0x2c
#define CMU_STATUS_HFRCOSEL		0x00000400
#define CMU_STATUS_HFXOSEL		0x00000800

#define CMU_HFPERCLKEN0		0x44
