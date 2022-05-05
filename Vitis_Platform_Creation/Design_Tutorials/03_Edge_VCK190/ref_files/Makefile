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

XSA=step0_bootable_design/build/vivado/vck190_custom_hw.xsa
IMG=step2_petalinux/build/petalinux/images/linux/image.ub
XPFM=step3_pfm/platform_repo/vck190_custom/export/vck190_custom/vck190_custom.xpfm

.PHONY: all clean step0 step2 step3 
all: step0 step2 step3 step4

step0: $(XSA)

$(XSA):
	$(MAKE) -C step0_bootable_design all

step2: $(IMG)

$(IMG): $(XSA) 
	$(MAKE) -C step2_petalinux all

step3: $(XPFM)

$(XPFM): $(XSA) $(IMG)
	$(MAKE) -C step3_pfm all

step4: $(XPFM) $(IMG)
	$(MAKE) -C step4_validate all

clean:
	$(MAKE) -C step0_bootable_design clean
	$(MAKE) -C step2_petalinux clean
	$(MAKE) -C step3_pfm clean
	$(MAKE) -C step4_validate clean
