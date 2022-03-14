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

ECHO=@echo

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the IPs used in this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

# =========================================================
# DEVICE SELECTION
# =========================================================
TARGET      ?= hw
DEVICE_NAME ?= xcvc1902-vsva2197-2MP-e-S

IP_TCL      := ./pack_kernel.tcl
KERNEL      := subtractor
KERNEL_SRC  := ./src/* ./${KERNEL}.xdc
XO          := ../xo_${TARGET}/${KERNEL}.xo

.PHONY: help all clean

all: ${XO}

${XO}: ${IP_TCL} ${KERNEL_SRC}
	vivado -mode batch -source $(IP_TCL) -tclargs $(KERNEL) $(DEVICE_NAME) $(XO)

clean:
	$(RM) -rf build .Xil
	$(RM) -rf $(XO) vivado.* *.xml *.jou *.log

