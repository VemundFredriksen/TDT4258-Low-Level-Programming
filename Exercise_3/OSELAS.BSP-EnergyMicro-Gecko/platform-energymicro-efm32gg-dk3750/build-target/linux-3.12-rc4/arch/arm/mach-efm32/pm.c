#include <linux/init.h>
#include <linux/suspend.h>

#include <asm/io.h>
#include <asm/v7m.h>

#include "cmu.h"

#define CMU_BASE IOMEM(0x400c8000)

#define scb_writel(val, addroff)	writel(val, BASEADDR_V7M_SCB + addroff)

static int efm32_suspend_enter(suspend_state_t state)
{
	u32 cmu_status = readl(CMU_BASE + CMU_STATUS);

	/*
	 * setting SLEEPDEEP makes the efm32 enter EM2 or EM3 (iff both
	 * LFACLK and LFBCLK are off).
	 */
	scb_writel(V7M_SCB_SCR_SLEEPDEEP, V7M_SCB_SCR);

	cpu_do_idle();

	scb_writel(0, V7M_SCB_SCR);

	/*
	 * deep sleep disables the HF oscilator, reenable it if it was on
	 * before.
	 */
	if (cmu_status & CMU_STATUS_HFXOSEL) {
		writel(CMU_OSCENCMD_HFXOEN, CMU_BASE + CMU_OSCENCMD);
		writel(CMU_CMD_HFCLKSEL_HFXO, CMU_BASE + CMU_CMD);
	}

	return 0;
}

static const struct platform_suspend_ops efm32_suspend_ops = {
	.valid = suspend_valid_only_mem,
	.enter = efm32_suspend_enter,
};

static int __init efm32_pm_init(void)
{
	suspend_set_ops(&efm32_suspend_ops);

	return 0;
}
arch_initcall(efm32_pm_init);
