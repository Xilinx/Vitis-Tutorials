# Copyright 2020 Xilinx Inc.
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

# ---------------------------------
# Target all: Generate the Vitis platform and verify it including base bootable design
# ---------------------------------

# Board: VCK190, VCK190_ES1
export BOARD=VCK190
PLATFORM_NAME=vck190_custom_dfx


XSA=./step1_hw/build/vivado/$(PLATFORM_NAME)_static.xsa
XPFM=./step2_sw/build/pfm/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm

.PHONY: all clean step1 step2 step3 
all: step1 step2 step3

step1: $(XSA)

$(XSA):
	echo "Step 1: Generating XSA $(XSA)"
	$(MAKE) -C step1_hw all

step2: $(XPFM)

$(XPFM): $(XSA) 
	echo "Step 2: Generating XPFM $(XPFM)"
	$(MAKE) -C step2_sw all



step3: $(XPFM)
	echo "Step 3: Validating the platform"
	$(MAKE) -C step3_validate all

clean:
	$(MAKE) -C step1_hw clean
	$(MAKE) -C step2_sw clean
	$(MAKE) -C step3_validate clean
