/*
 * Copyright (C) 2012-2013 Uwe Kleine-Koenig for Pengutronix
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/bug.h>

#include <asm/io.h>
#include <asm/barrier.h>

#include "common.h"

#define MEM_INFO_FLASH		(void __iomem *)0x0fe081f8
#define MEM_INFO_RAM		(void __iomem *)0x0fe081fa
#define PART_NUMBER		(void __iomem *)0x0fe081fc
#define PART_FAMILY		(void __iomem *)0x0fe081fe
#define PROD_REV		(void __iomem *)0x0fe081ff

static const struct efm32_family_mapping {
	u8 id;
	const char *name;
	const char *abbrev;
} efm32_family_mapping[] __initconst = {
	{
		.id = 71,
		.name = "Gecko",
		.abbrev = "G",
	}, {
		.id = 72,
		.name = "Giant Gecko",
		.abbrev = "GG",
	}, {
		.id = 73,
		.name = "Tiny Gecko",
		.abbrev = "TG",
	}, {
		.id = 74,
		.name = "Leopard Gecko",
		.abbrev = "LG",
	}, {
		.id = 75,
		.name = "Zero Gecko",
		.abbrev = "ZG",
	},
};

static const char *__init efm32_get_familyname(u8 id)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(efm32_family_mapping) &&
			efm32_family_mapping[i].id <= id; ++i) {
		if (efm32_family_mapping[i].id == id)
			return efm32_family_mapping[i].name;
	}

	return "unknown";
}

static const char *__init efm32_get_familyabbrev(u8 id)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(efm32_family_mapping) &&
			efm32_family_mapping[i].id <= id; ++i) {
		if (efm32_family_mapping[i].id == id)
			return efm32_family_mapping[i].abbrev;
	}

	return "unknown";
}

static char revbuf[4] __initdata = "";
static volatile const u32 * const romtable = (void *)0xe00fffe0;

static const char *__init efm32_get_rev(void)
{
	if (revbuf[0] == '\0') {
		u32 major = romtable[0] & 0x3f;
		u32 minor = (romtable[2] & 0xf0) | ((romtable[3] >> 4) & 0x0f);

		if (minor < 25)
			sprintf(revbuf, "%u%c", major, 'A' + minor);
		else {
			revbuf[0] = '?';
			revbuf[1] = '\0';
		}
	}
	return revbuf;
}

void __init efm32_print_cpuinfo(void)
{
	u16 partno = __raw_readw(PART_NUMBER);
	u8 family = __raw_readb(PART_FAMILY);
	u8 rev = __raw_readb(PROD_REV);
	u16 flashsize = __raw_readw(MEM_INFO_FLASH);
	u16 raminfo = __raw_readw(MEM_INFO_RAM);

	pr_info("Energy Micro %s, EFM32%s%hdF%hd (rev %s, prodrev %hhd), %hd kB RAM\n",
			efm32_get_familyname(family),
			efm32_get_familyabbrev(family), partno,
			flashsize, efm32_get_rev(), rev, raminfo);
}

static const struct {
	u32 value;
	u32 mask;
	const char *cause;
} efm32_reset_causes[] __initconst = {
	{
		.value = 0x0001,
		.mask = 0x0001,
		.cause = "A Power-on Reset has been performed",
	}, {
		.value = 0x0002,
		.mask = 0x0083,
		.cause = "A Brown-out has been detected on the unregulated power",
	}, {
		.value = 0x0004,
		.mask = 0x001f,
		.cause = "A Brown-out has been detected on the regulated power",
	}, {
		.value = 0x0008,
		.mask = 0x000b,
		.cause = "An external reset has been applied",
	}, {
		.value = 0x0010,
		.mask = 0x0013,
		.cause = "A watchdog reset has occurred",
	}, {
		.value = 0x0020,
		.mask = 0x07ff,
		.cause = "A lockup reset has occurred",
	}, {
		.value = 0x0040,
		.mask = 0x07df,
		.cause = "A system request reset has occurred",
	}, {
		.value = 0x0080,
		.mask = 0x0799,
		.cause = "The system as woken up from EM4",
	}, {
		.value = 0x0180,
		.mask = 0x799,
		.cause = "The system as woken up from EM4 on an EM4 wakeup reset request from pin",
	}, {
		.value = 0x0200,
		.mask = 0x079f,
		.cause = "A Brown-out has been detected on Analog Power Domain 0 (AVDD0)",
	}, {
		.value = 0x0400,
		.mask = 0x079f,
		.cause = "A Brown-out has been detected on Analog Power Domain 1 (AVDD1)",
	}, {
		.value = 0x0800,
		.mask = 0x0800,
		.cause = "A Brown-out has been detected by the Backup BOD on VDD_DREG",
	}, {
		.value = 0x1000,
		.mask = 0x1000,
		.cause = "A Brown-out has been detected by the Backup BOD on BU_VIN",
	}, {
		.value = 0x2000,
		.mask = 0x2000,
		.cause = "A Brown-out has been detected by the Backup BOD on unregulated power",
	}, {
		.value= 0x4000,
		.mask = 0x4000,
		.cause = "A Brown-out has been detected by the Backup BOD on regulated power",
	}, {
		.value = 0x8000,
		.mask = 0x8000,
		.cause = "The system has been in Backup mode",
	},
};

void __init efm32_print_reset_cause(void)
{
	u32 rmu_rstcause = __raw_readl((void __iomem *)0x400ca004);
	int i;

	pr_info("Reset Cause: 0x%08x\n", rmu_rstcause);

	for (i = 0; i < ARRAY_SIZE(efm32_reset_causes); ++i) {
		if ((rmu_rstcause & efm32_reset_causes[i].mask) ==
				efm32_reset_causes[i].value)
			pr_info(" `-> %s.\n", efm32_reset_causes[i].cause);
	}

	/* clear RMU_RSTCAUSE */
	__raw_writel(1, (void __iomem *)0x400ca008);
	__raw_writel(1, (void __iomem *)0x400c6024);
	__raw_writel(0, (void __iomem *)0x400c6024);
}
