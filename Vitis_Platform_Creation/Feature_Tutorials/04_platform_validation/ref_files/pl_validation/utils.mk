#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/
PROFILE := no

#Generates profile summary report
ifeq ($(PROFILE), yes)
VPP_LDFLAGS += --profile.data all:all:all
endif

DEBUG := no

#Generates debug summary report
ifeq ($(DEBUG), yes)
VPP_LDFLAGS += --dk list_ports
endif

#Checks for XILINX_VITIS
check-vitis:
ifndef XILINX_VITIS
	$(error XILINX_VITIS variable is not set, please set correctly using "source <Vitis_install_path>/Vitis/<Version>/settings64.sh" and rerun)
endif

#Checks for XILINX_XRT
check-xrt:
ifeq ($(HOST_ARCH), x86)
ifndef XILINX_XRT
	$(error XILINX_XRT variable is not set, please set correctly using "source /opt/xilinx/xrt/setup.sh" and rerun)
endif
else
ifndef XILINX_VITIS
	$(error XILINX_VITIS variable is not set, please set correctly using "source <Vitis_install_path>/Vitis/<Version>/settings64.sh" and rerun)
endif
endif

#Setting CXX
CXX := g++

# Cleaning stuff
RM = rm -f
RMDIR = rm -rf

ECHO:= @echo

docs: README.rst

README.rst: description.json
	$(XF_PROJ_ROOT)/common/utility/readme_gen/readme_gen.py description.json
