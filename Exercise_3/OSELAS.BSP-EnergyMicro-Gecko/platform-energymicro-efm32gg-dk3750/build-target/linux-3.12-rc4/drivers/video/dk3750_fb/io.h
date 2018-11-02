#ifndef IO_H
#define IO_H

#include "bsp_dk_bcreg_3201.h"

#define UIF_AEM  ((volatile uint16_t*)(BC_REGISTER_BASE + 0x0e))
#define ARB_CTRL ((volatile uint16_t*)(BC_REGISTER_BASE + 0x16))
#define DISPLAY_CTRL ((volatile uint16_t*)(BC_REGISTER_BASE + 0x12))

#endif
