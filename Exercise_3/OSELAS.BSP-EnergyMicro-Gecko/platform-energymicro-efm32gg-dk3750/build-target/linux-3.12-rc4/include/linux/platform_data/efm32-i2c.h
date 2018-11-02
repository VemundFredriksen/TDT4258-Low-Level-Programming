#ifndef __LINUX_PLATFORM_DATA_EFM32_I2C_H__
#define __LINUX_PLATFORM_DATA_EFM32_I2C_H__

#include <linux/types.h>

/**
 * struct efm32_i2c_pdata
 * @location: pinmux location for the I/O pins (to be written to the ROUTE
 * 	register)
 */
struct efm32_i2c_pdata {
	u8 location;
	unsigned long frequency; /* in Hz */
};
#endif /* ifndef __LINUX_PLATFORM_DATA_EFM32_I2C_H__ */
