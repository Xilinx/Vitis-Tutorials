
xo: ./$(builddir)/vadd_$(TARGET).xo
./$(builddir)/vadd_$(TARGET).xo: $(FPGA_SRC_CPP)
	v++ -c -g -t $(TARGET) -R 1 -k vadd \
		--platform $(PLATFORM) \
                --profile.data all:all:all \
                --profile.stall all:all:all \
		-DDATA_ELEMENTS=$(dwords) \
		-DBLENGTH=$(bl) \
		--save-temps \
		--temp_dir ./$(builddir)/temp_dir \
		--report_dir ./$(builddir)/report_dir \
		--log_dir ./$(builddir)/log_dir \
		-I$(SRCDIR) \
		$(FPGA_SRC_CPP) \
		-o ./$(builddir)/vadd_$(TARGET).xo

xclbin: ./$(builddir)/vadd_$(TARGET).xclbin
./$(builddir)/vadd_$(TARGET).xclbin: ./$(builddir)/vadd_$(TARGET).xo
	@echo "Running target to generate xclbin file"
	v++ -l -g -t $(TARGET) -R 1 \
		--platform $(PLATFORM) \
                --profile.data all:all:all \
                --profile.stall all:all:all \
		--save-temps \
		--temp_dir ./$(builddir)/temp_dir \
		--report_dir ./$(builddir)/report_dir \
		--log_dir ./$(builddir)/log_dir \
		--config $(builddir)/$(memtype)_connectivity.cfg \
		-I$(SRCDIR) \
		./$(builddir)/vadd_$(TARGET).xo \
		-o ./$(builddir)/vadd_$(TARGET).xclbin

host: $(builddir)/host
$(builddir)/host: $(HOST_SRC_CPP)
	mkdir -p $(builddir)
	g++ -D__USE_XOPEN2K8 -D__USE_XOPEN2K8 \
		-I$(XILINX_XRT)/include/ \
		-I$(SRCDIR) \
		-O3 -Wall -fmessage-length=0 -std=c++11\
		$(HOST_SRC_CPP) \
		-L$(XILINX_XRT)/lib/ \
		-lxilinxopencl -pthread -lrt \
		-o $(builddir)/host

$(builddir)/$(EMCONFIG_FILE):
	emconfigutil --nd 1  --platform $(PLATFORM) --od $(builddir)

clean:
	rm -rf temp_dir log_dir ./build/hbm*/.log ./build/hbm*/*summary ./build/*.info report_dir *log host vadd* *.csv *summary .run .Xil vitis* *jou xilinx*

cleanall:
	rm -rf ./build 
