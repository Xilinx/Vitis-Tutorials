PLATFORM = ${PLATFORM_REPO_PATHS}/xilinx_vck190_es1_base_202020_1/xilinx_vck190_es1_base_202020_1.xpfm

.PHONY: clean

all:$(subst .cpp,.xo,$(wildcard *.cpp)) ../Work/temp/packet_ids_c.h
%.xo: %.cpp
	cp ../Work/temp/packet_ids_c.h .
	v++ -c --platform ${PLATFORM} -k $(basename $<) --kernel_frequency 250 $< -o $@ --verbose --save-temps

clean:
	rm -rf _x *.xo* *.log
