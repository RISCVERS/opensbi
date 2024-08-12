#
# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (c) 2024 Beijing Institute of Open Source Chip (BOSC)
#
# Authors:
#   Shaoqing Qin <qinshaoqing@bosc.ac.cn>
#

# Compiler flags
platform-cppflags-y =
platform-cflags-y =
platform-asflags-y =
platform-ldflags-y =

# Objects to build
platform-objs-y += platform.o

platform-objs-y += kmh.o
platform-varprefix-kmh.o = dt_kmh
platform-padding-kmh.o = 2048

# Blobs to build
FW_TEXT_START=0x80000000
FW_DYNAMIC=y
FW_JUMP=y
FW_JUMP_ADDR=$(shell printf "0x%X" $$(($(FW_TEXT_START) + 0x200000)))
FW_JUMP_FDT_ADDR=$(shell printf "0x%X" $$(($(FW_TEXT_START) + 0x2200000)))
FW_PAYLOAD=y
FW_PAYLOAD_OFFSET=0x200000
FW_PAYLOAD_ALIGN=0x1000
FW_PAYLOAD_FDT_ADDR=0x82200000
