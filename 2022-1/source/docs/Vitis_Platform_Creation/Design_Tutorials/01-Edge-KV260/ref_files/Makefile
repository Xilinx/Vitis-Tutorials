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

.PHONY: all clean
all: step1 step2 step3 step4

step1:
	$(MAKE) -C step1_vivado all

step2:
	$(MAKE) -C step2_petalinux all

step3:
	$(MAKE) -C step3_pfm all

step4:
	$(MAKE) -C step4_validate all

clean:
	$(MAKE) -C step1_vivado clean
	$(MAKE) -C step2_petalinux clean
	$(MAKE) -C step3_pfm clean
	$(MAKE) -C step4_validate clean
