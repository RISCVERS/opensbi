/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2024 Beijing Institute of Open Source Chip (BOSC)
 *
 * Authors:
 *   Shaoqing Qin <qinshaoqing@bosc.ac.cn>
 */
#ifndef _KMH_PLATFORM_H_
#define _KMH_PLATFORM_H_

#include <sbi/riscv_io.h>

#define KMH_HART_COUNT		1

#define KMH_UART_ADDR	  0x310b0000ULL
#define KMH_UART_FREQ	  50000000
#define KMH_UART_BAUDRATE  115200
#define KMH_UART_REG_SHIFT			2
#define KMH_UART_REG_WIDTH			4
#define KMH_UART_REG_OFFSET			0

#define KMH_ACLINT_MTIMER_FREQ	500000
#define KMH_CLK0_FREQ		26000000UL
#define KMH_PLIC_NUM_SOURCES	65

/* Registers base address */
#define KMH_SYSCTL_BASE_ADDR	0x50440000ULL
#define KMH_CLINT_BASE_ADDR	0x38000000ULL
#define KMH_ACLINT_MSWI_ADDR	\
		(KMH_CLINT_BASE_ADDR + CLINT_MSWI_OFFSET)
#define KMH_ACLINT_MTIMER_ADDR \
		(KMH_CLINT_BASE_ADDR + CLINT_MTIMER_OFFSET)
#define KMH_PLIC_BASE_ADDR	0x0C000000ULL
#define KMH_PLIC_BASE_SIZE	(0x200000ULL + (KMH_HART_COUNT * 0x1000))

#endif /* _KMH_PLATFORM_H_ */
