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

.PHONY: all clean step1 step2 step3 petalinux_customization
all: step1 step2 step3 petalinux_customization

XSA=step1_vivado/build/vivado/zcu104_custom_platform_hw.xsa
IMG=step2_pfm/xilinx-zynqmp-common-v2022.1/Image
XPFM=step2_pfm/zcu104_custom/export/zcu104_custom/zcu104_custom.xpfm

# Step 1: Generate XSA with Vivado
step1: $(XSA)

$(XSA):
	$(MAKE) -C step1_vivado all


# Step 2: Generate Platform
step2: $(XPFM)

$(XPFM): $(XSA) 
	$(MAKE) -C step2_pfm all

# Step 3: Validate Design
step3: $(XPFM)
	$(MAKE) -C step3_validate all

clean:
	$(MAKE) -C step1_vivado clean
	$(MAKE) -C step2_pfm clean
	$(MAKE) -C step3_validate clean
