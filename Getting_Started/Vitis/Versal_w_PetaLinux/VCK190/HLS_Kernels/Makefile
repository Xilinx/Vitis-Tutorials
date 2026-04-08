#Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

.PHONY: all clean

PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202610_1/xilinx_vck190_base_202610_1.xpfm

s2mm = ./s2mm/s2mm.xo
mm2s = ./mm2s/mm2s.xo
all: $(s2mm) $(mm2s)


$(s2mm):
	v++ -c --mode hls --platform $(PLATFORM) --config ./s2mm.cfg 

$(mm2s): 
	v++ -c --mode hls --platform $(PLATFORM) --config ./mm2s.cfg 

clean:
	rm -rf ./hls/ ./mm2s/ ./s2mm/
