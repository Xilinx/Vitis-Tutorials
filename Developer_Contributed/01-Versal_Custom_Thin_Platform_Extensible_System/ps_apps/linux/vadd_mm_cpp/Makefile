#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO             := @echo

SYSROOT          := ../../../linux/sysroot/sysroots/cortexa72-cortexa53-xilinx-linux

APP_NAME         := vadd_mm_cpp
APP_EXE          := ../exe/${APP_NAME}.exe
APP_SRC          := ./src/${APP_NAME}.cpp
APP_SRC_DEP      := ./src/*

GCC              ?= aarch64-linux-gnu-g++

GCC_INCLUDES     := -I${SYSROOT}/usr/include/xrt \
                    -I${SYSROOT}/usr/include \
                    -I${XILINX_VITIS}/include

GCC_C_FLAGS      := -O -Wall -std=c++14 \
                    --sysroot=${SYSROOT}

GCC_L_FLAGS      := -lxrt_core -lxrt_coreutil \
                    -L${SYSROOT}/usr/lib \
                    --sysroot=${SYSROOT}

.PHONY: clean help all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the vadd_cpp ps_app used in this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: $(APP_EXE)

$(APP_EXE): $(APP_SRC_DEP) ${SYSROOT}
	${GCC} ${GCC_C_FLAGS} $(APP_SRC) ${GCC_INCLUDES} $(GCC_L_FLAGS) -o $(APP_EXE)

clean:
	rm -rf *.o $(APP_EXE)
