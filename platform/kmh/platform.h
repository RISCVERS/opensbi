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

#define MY_UART_ADDR	  0x310b0000ULL
#define MY_UART_FREQ	  50000000
#define MY_UART_BAUDRATE  115200
#define MY_UART_REG_SHIFT			2
#define MY_UART_REG_WIDTH			4
#define MY_UART_REG_OFFSET			0

#endif /* _KMH_PLATFORM_H_ */
