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

.PHONY: all clean step1 step2 step3 step4
all: step1 step2 step3 step4

XSA=step1_vivado/build/vivado/zcu104_custom_platform_hw.xsa
IMG=step2_petalinux/build/petalinux/images/image.ub
XPFM=step3_pfm/platform_repo/zcu104_custom_platform/export/zcu104_custom_platform/zcu104_custom_platform.xpfm

# Step 1: Generate XSA with Vivado
step1: $(XSA)

$(XSA):
	$(MAKE) -C step1_vivado all

# Step 2: Use PetaLinux to generate Software Components
step2: $(IMG)

$(IMG): $(XSA) 
	$(MAKE) -C step2_petalinux all

# Step 3: Generate Platform
step3: $(XPFM)

$(XPFM): $(XSA) $(IMG)
	$(MAKE) -C step3_pfm all

# Step 4: Validate Design
step4: $(XPFM)
	$(MAKE) -C step4_validate all

clean:
	$(MAKE) -C step1_vivado clean
	$(MAKE) -C step2_petalinux clean
	$(MAKE) -C step3_pfm clean
	$(MAKE) -C step4_validate clean
