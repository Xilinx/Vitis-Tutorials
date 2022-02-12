 #
 # Copyright 2021 Xilinx, Inc.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #

ECHO := @echo

APP_NAME     := ./vadd_ocl
APP_EXE      := ../exe/${APP_NAME}.exe
APP_SRC      := ./src/${APP_NAME}.cpp

SYSROOT      := ../../petalinux/sysroot/sysroots/cortexa72-cortexa53-xilinx-linux

GCC          ?= aarch64-linux-gnu-g++

GCC_INCLUDES := -I${SYSROOT}/usr/include/xrt \
                -I${SYSROOT}/usr/include \
                -I${XILINX_VITIS}/include

GCC_C_FLAGS  := -O -Wall -std=c++14 \
                --sysroot=${SYSROOT}

GCC_L_FLAGS  := -lxilinxopencl -lxrt_core -lxrt_coreutil \
                -L${SYSROOT}/usr/lib \
                --sysroot=${SYSROOT}

.PHONY: clean help all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the vadd_ocl ps_app used in this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: $(APP_EXE)

$(APP_EXE): $(APP_SRC)
	${GCC} ${GCC_C_FLAGS} $(APP_SRC) ${GCC_INCLUDES} $(GCC_L_FLAGS) -o $(APP_EXE)

clean:
	rm -rf *.o $(APP_EXE)
