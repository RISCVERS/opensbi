/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2024 Beijing Institute of Open Source Chip (BOSC).
 *
 * Authors:
 *   Shaoqing Qin <qinshaoqing@bosc.ac.cn>
 */

#include <sbi/riscv_asm.h>
#include <sbi/riscv_encoding.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_const.h>
#include <sbi/sbi_platform.h>
#include <sbi/sbi_system.h>
#include <sbi_utils/fdt/fdt_helper.h>
#include <sbi_utils/fdt/fdt_fixup.h>
#include <sbi_utils/ipi/aclint_mswi.h>
#include <sbi_utils/irqchip/plic.h>
#include <sbi_utils/timer/aclint_mtimer.h>
#include <sbi_utils/serial/uart8250.h>
#include "platform.h"

static u32 generic_hart_index2id[SBI_HARTMASK_MAX_BITS] = {0};

static struct aclint_mswi_data mswi = {
	.addr = KMH_ACLINT_MSWI_ADDR,
	.size = ACLINT_MSWI_SIZE,
	.first_hartid = 0,
	.hart_count = KMH_HART_COUNT,
};

static struct aclint_mtimer_data mtimer = {
	.mtime_freq = KMH_ACLINT_MTIMER_FREQ,
	.mtime_addr = KMH_ACLINT_MTIMER_ADDR +
		      ACLINT_DEFAULT_MTIME_OFFSET,
	.mtime_size = ACLINT_DEFAULT_MTIME_SIZE,
	.mtimecmp_addr = KMH_ACLINT_MTIMER_ADDR +
			 ACLINT_DEFAULT_MTIMECMP_OFFSET,
	.mtimecmp_size = ACLINT_DEFAULT_MTIMECMP_SIZE,
	.first_hartid = 0,
	.hart_count = KMH_HART_COUNT,
	.has_64bit_mmio = true,
};

static int kmh_early_init(bool cold_boot)
{
	if (!cold_boot)
		return 0;

	return uart8250_init(KMH_UART_ADDR,
			     KMH_UART_FREQ,
			     KMH_UART_BAUDRATE,
			     KMH_UART_REG_SHIFT,
			     KMH_UART_REG_WIDTH,
			     KMH_UART_REG_OFFSET);
}

static int kmh_final_init(bool cold_boot)
{
	void *fdt;

	if (!cold_boot)
		return 0;

	fdt = fdt_get_address();

	fdt_cpu_fixup(fdt);
	fdt_fixups(fdt);

	return 0;
}


static int kmh_ipi_init(bool cold_boot)
{
	int ret;

	if (cold_boot) {
		ret = aclint_mswi_cold_init(&mswi);
		if (ret)
			return ret;
	}

	return aclint_mswi_warm_init();
}

static int kmh_timer_init(bool cold_boot)
{
	int ret;

	if (cold_boot) {
		ret = aclint_mtimer_cold_init(&mtimer, NULL);
		if (ret)
			return ret;
	}

	return aclint_mtimer_warm_init();
}

const struct sbi_platform_operations platform_ops = {
	.early_init	= kmh_early_init,
	.final_init	= kmh_final_init,
	.ipi_init  = kmh_ipi_init,
	.timer_init	   = kmh_timer_init,
};

const struct sbi_platform platform = {
	.opensbi_version	= OPENSBI_VERSION,
	.platform_version   	= SBI_PLATFORM_VERSION(0x0, 0x01),
	.name			= "kmh",
	.features		= SBI_PLATFORM_DEFAULT_FEATURES,
	.hart_count		= KMH_HART_COUNT,
	.hart_index2id		= generic_hart_index2id,
	.hart_stack_size	= SBI_PLATFORM_DEFAULT_HART_STACK_SIZE,
	.heap_size		= SBI_PLATFORM_DEFAULT_HEAP_SIZE(KMH_HART_COUNT),
	.platform_ops_addr	= (unsigned long)&platform_ops
};
