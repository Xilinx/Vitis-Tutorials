OUTPUT=../platform_repo/tmp
HW_PATH=$(OUTPUT)/vivado
VIVADO=$(XILINX_VIVADO)/bin/vivado

all: hw

.PHONY: all hw clean

hw:
	mkdir -p $(HW_PATH)
	$(VIVADO) -mode batch -notrace -source $(PLATFORM)_xsa.tcl

clean:
	$(RM) -r vivado.* $(PLATFORM) $(PLATFORM).xsa  xilinx_zcu104_vcu_202010_1 xilinx_zcu104_vcu_202010_1.xsa .Xil *log *jou
