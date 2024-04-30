#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

PART_NUMBER ?= xcvc1902-vsva2197-2MP-e-S
XO_NAME     ?= demo_ip

.PHONY: all xo clean cleanall
all:	$(XO_NAME).xo

$(XO_NAME).xo : rtl/mem/*.* rtl/tst_din/*.v rtl/tst_dout/*.v rtl/wrp_shff/*.v rtl/wrp_din/*.v
	vivado -mode batch -source rtlpacker_generic.tcl -tclargs $(PART_NUMBER) $(XO_NAME) wrp_din  1  0 0 0
	vivado -mode batch -source rtlpacker_generic.tcl -tclargs $(PART_NUMBER) $(XO_NAME) wrp_shff 1  0 0 0
	vivado -mode batch -source rtlpacker_generic.tcl -tclargs $(PART_NUMBER) $(XO_NAME) tst_din  1  1 0 0
	vivado -mode batch -source rtlpacker_generic.tcl -tclargs $(PART_NUMBER) $(XO_NAME) tst_dout 1  1 1 0

clean:
		/bin/rm -rf *.jou *.log .crashReporter* .Xil

cleanall:	clean
		/bin/rm -rf $(XO_NAME).xo ip_*


