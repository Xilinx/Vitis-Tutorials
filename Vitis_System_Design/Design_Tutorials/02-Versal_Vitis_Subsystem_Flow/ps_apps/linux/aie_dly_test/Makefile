#
# Copyright (C) 2021-2022, Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO             := @echo

SYSROOT          := ../../../linux/sysroot/sysroots/cortexa72-cortexa53-amd-linux
SYSROOT_SETUP    := ../../../linux/sysroot/environment-setup-cortexa72-cortexa53-amd-linux

APP_NAME         := $(notdir $(CURDIR))
APP_EXE          := ../exe/${APP_NAME}.exe
APP_SRC          := ./src/${APP_NAME}.cpp
APP_SRC_DEP      := ./src/*
APP_SRC_CTRL     := ./../../../vss/ip/aie/datamove_app_ws/Work/ps/c_rts/aie_control_xrt.cpp

GCC              ?= aarch64-amd-linux-g++
#GCC              ?= aarch64-linux-gnu-g++

GCC_INCLUDES     := -I${SYSROOT}/usr/include \
                    -I${SYSROOT}/usr/include/xrt \
                    -I${SYSROOT}/usr/include/xaiengine \
                    -I${XILINX_VITIS}/aietools/include \
                    -I../../../vss/ip/aie/src \
                    -I../../../vss/ip/aie/

GCC_C_FLAGS      := -O -Wall -std=c++17 -DXAIE_DEBUG \
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
	. ${SYSROOT_SETUP} && ${GCC} ${GCC_C_FLAGS} $(APP_SRC) $(APP_SRC_CTRL) ${GCC_INCLUDES} $(GCC_L_FLAGS) -o $(APP_EXE)

clean:
	rm -rf *.o $(APP_EXE)


