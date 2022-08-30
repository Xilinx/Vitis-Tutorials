
./$(BUILDDIR)/runOnfpga_$(TARGET).xo: $(SRCDIR)/hls_stream_utils.h $(HOST_SRC_FPGA)
	v++ -c -g -t $(TARGET) -R 1 -k runOnfpga \
		--platform $(PLATFORM) \
        --profile.data all:all:all \
        --profile.stall all:all:all \
		--profile.trace_memory DDR[3] \
		--save-temps \
		--temp_dir ./$(BUILDDIR)/temp_dir \
		--report_dir ./$(BUILDDIR)/report_dir \
		--log_dir ./$(BUILDDIR)/log_dir \
		-I$(SRCDIR) \
		-DPARALLELISATION=$(PF) \
		$(HOST_SRC_FPGA) \
		-o ./$(BUILDDIR)/runOnfpga_$(TARGET).xo

ifeq ($(STEP),multiDDR)
./$(BUILDDIR)/runOnfpga_$(TARGET).xclbin: ./$(BUILDDIR)/runOnfpga_$(TARGET).xo
	v++ -l -g -t $(TARGET) -R 1 \
		--platform $(PLATFORM) \
        --profile.data all:all:all \
        --profile.stall all:all:all \
		--profile.trace_memory DDR[3] \
		--temp_dir ./$(BUILDDIR)/temp_dir \
		--report_dir ./$(BUILDDIR)/report_dir \
		--log_dir ./$(BUILDDIR)/log_dir \
		--config $(CURRENT_DIR)/connectivity.cfg \
		-I$(SRCDIR) \
		./$(BUILDDIR)/runOnfpga_$(TARGET).xo \
		-o ./$(BUILDDIR)/runOnfpga_$(TARGET).xclbin

else

./$(BUILDDIR)/runOnfpga_$(TARGET).xclbin: ./$(BUILDDIR)/runOnfpga_$(TARGET).xo
	v++ -l -g -t $(TARGET) -R 1 \
		--platform $(PLATFORM) \
        --profile.data all:all:all \
        --profile.stall all:all:all \
		--profile.trace_memory DDR[3] \
		--temp_dir ./$(BUILDDIR)/temp_dir \
		--report_dir ./$(BUILDDIR)/report_dir \
		--log_dir ./$(BUILDDIR)/log_dir \
		-I$(SRCDIR) \
		./$(BUILDDIR)/runOnfpga_$(TARGET).xo \
		-o ./$(BUILDDIR)/runOnfpga_$(TARGET).xclbin

endif

host: $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.h
	mkdir -p $(BUILDDIR)
	g++ -D__USE_XOPEN2K8 -D__USE_XOPEN2K8 \
		-I$(XILINX_XRT)/include/ \
		-I$(SRCDIR) \
		-O3 -Wall -fmessage-length=0 -std=c++11\
		$(HOST_SRC_CPP) \
		-L$(XILINX_XRT)/lib/ \
		-lxilinxopencl -pthread -lrt \
		-o $(BUILDDIR)/host

emconfig.json:
	cp $(SRCDIR)/emconfig.json .

xclbin: ./$(BUILDDIR)/runOnfpga_$(TARGET).xclbin

xo: ./$(BUILDDIR)/runOnfpga_$(TARGET).xo

clean:
	rm -rf temp_dir log_dir ../build report_dir *log host runOnfpga* *.csv *summary .run .Xil vitis* *jou xilinx*
