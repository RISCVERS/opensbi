#
# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (c) 2024 Beijing Institute of Open Source Chip (BOSC).
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

# Blobs to build
FW_DYNAMIC=y
FW_JUMP=y
# This needs to be 2MB aligned for 64-bit system
FW_JUMP_OFFSET=0x400000
FW_JUMP_FDT_OFFSET=0x200000
FW_PAYLOAD=y
# This needs to be 2MB aligned for 64-bit system
FW_PAYLOAD_OFFSET=0x400000
FW_PAYLOAD_FDT_OFFSET=$(FW_JUMP_FDT_OFFSET)
