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
# Target main: Generate the Vitis platform and verify it without base bootable design
# ---------------------------------
.PHONY: all clean step0 step2 step3 
all: step0 step2 step3 

step0:
	$(MAKE) -C step0_bootable_design all

step2:
	$(MAKE) -C step2_petalinux all

step3:
	$(MAKE) -C step3_pfm all

clean:
	$(MAKE) -C step0_bootable_design clean
	$(MAKE) -C step1_vivado clean
	$(MAKE) -C step2_petalinux clean
	$(MAKE) -C step3_pfm clean