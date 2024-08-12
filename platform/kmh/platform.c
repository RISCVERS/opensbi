/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2024 Beijing Institute of Open Source Chip (BOSC)
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

extern const char dt_kmh_start[];
static u32 generic_hart_index2id[SBI_HARTMASK_MAX_BITS] = {0};

unsigned long fw_platform_init(unsigned long arg0, unsigned long arg1,
				unsigned long arg2, unsigned long arg3,
				unsigned long arg4)
{
	return (unsigned long)&dt_kmh_start[0];
}
static struct plic_data plic = {
	.addr = 0x3c000000,
	.size = 0x4000000,
	.num_src = 0x40,
};

static struct aclint_mswi_data mswi = {
	.addr = 0x38000000,
	.size = 0x4000,
	.first_hartid = 0,
	.hart_count = 1,
};

static struct aclint_mtimer_data mtimer = {
	.mtime_freq = 0x7a120,
	.mtime_addr = 0x3800bff8,
	.mtime_size = 0x4008,
	.mtimecmp_addr = 0x38004000,
	.mtimecmp_size = 0x7ff8,
	.first_hartid = 0,
	.hart_count = 1,
	.has_64bit_mmio = true,
};

static int kmh_system_reset_check(u32 type, u32 reason)
{
	return 1;
}

static void kmh_system_reset(u32 type, u32 reason)
{
}

static struct sbi_system_reset_device kmh_reset = {
	.name = "kmh_reset",
	.system_reset_check = kmh_system_reset_check,
	.system_reset = kmh_system_reset
};

static int kmh_early_init(bool cold_boot)
{
	if (cold_boot)
		sbi_system_reset_add_device(&kmh_reset);

	return 0;
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

static int kmh_console_init(void)
{
	return uart8250_init(MY_UART_ADDR,
			     MY_UART_FREQ,
			     MY_UART_BAUDRATE,
			     MY_UART_REG_SHIFT,
			     MY_UART_REG_WIDTH,
			     MY_UART_REG_OFFSET);
}

static int kmh_irqchip_init(bool cold_boot)
{
	int rc;
	u32 hartid = current_hartid();

	if (cold_boot) {
		rc = plic_cold_irqchip_init(&plic);
		if (rc)
			return rc;
	}

	return plic_warm_irqchip_init(&plic, hartid * 2, hartid * 2 + 1);
}

static int kmh_ipi_init(bool cold_boot)
{
	int rc;

	if (cold_boot) {
		rc = aclint_mswi_cold_init(&mswi);
		if (rc)
			return rc;
	}

	return aclint_mswi_warm_init();
}

static int kmh_timer_init(bool cold_boot)
{
	int rc;

	if (cold_boot) {
		rc = aclint_mtimer_cold_init(&mtimer, NULL);
		if (rc)
			return rc;
	}

	return aclint_mtimer_warm_init();
}

const struct sbi_platform_operations platform_ops = {
	.early_init	= kmh_early_init,

	.final_init	= kmh_final_init,

	.console_init	= kmh_console_init,

	.irqchip_init = kmh_irqchip_init,

	.ipi_init  = kmh_ipi_init,

	.timer_init	   = kmh_timer_init,
};

const struct sbi_platform platform = {
	.opensbi_version	= OPENSBI_VERSION,
	.platform_version   	= SBI_PLATFORM_VERSION(0x0, 0x01),
	.name			= "xiangshan",
	.features		= SBI_PLATFORM_DEFAULT_FEATURES,
	.hart_count		= 1,
	.hart_index2id		= generic_hart_index2id,
	.hart_stack_size	= SBI_PLATFORM_DEFAULT_HART_STACK_SIZE,
	.heap_size		=
			SBI_PLATFORM_DEFAULT_HEAP_SIZE(0),
	.platform_ops_addr	= (unsigned long)&platform_ops
};
