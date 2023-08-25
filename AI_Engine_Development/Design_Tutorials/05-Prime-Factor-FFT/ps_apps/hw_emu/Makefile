#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

ECHO             := @echo

APP_NAME         := host
APP_EXE          := ${APP_NAME}.exe
APP_SRC          := ${APP_NAME}.cpp
APP_SRC_DEP      := ./*
APP_SRC_CTRL     := ./../../aie/pfa1008/Work/ps/c_rts/aie_control_xrt.cpp

GCC              := ${XILINX_VITIS}/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++

GCC_INCLUDES     := -I${SYSROOT}/usr/include \
                    -I${SYSROOT}/usr/include/xrt \
                    -I${SYSROOT}/usr/include/xaiengine \
                    -I${XILINX_VITIS}/aietools/include \
                    -I../../aie/pfa1008 \
                    -I../../aie/dft7w \
                    -I../../aie/dft9w \
                    -I../../aie/dft16

GCC_C_FLAGS      := -O -Wall -std=c++14 -DXAIE_DEBUG \
                    -pthread \
                    --sysroot=${SYSROOT}

GCC_L_FLAGS      := -L$(SYSROOT)/usr/lib \
                    -L${XILINX_VITIS}/aietools/lib/aarch64.o \
                    -ladf_api_xrt -lxaiengine -lxrt_core -lxrt_coreutil \
                    --sysroot=$(SYSROOT)

.PHONY: clean help all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the aie_dly_test ps_app used in this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."


all: $(APP_EXE)

$(APP_EXE): $(APP_SRC_DEP) $(APP_SRC_CTRL) ${SYSROOT}
	${GCC} ${GCC_C_FLAGS} $(APP_SRC) $(APP_SRC_CTRL) ${GCC_INCLUDES} $(GCC_L_FLAGS) -o $(APP_EXE)

clean:
	rm -rf *.o $(APP_EXE)
