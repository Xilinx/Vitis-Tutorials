

host_gprof: $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.h
	g++ -D__USE_XOPEN2K8 -D__USE_XOPEN2K8 \
		-I$(SRCDIR) \
		-O0 -Wall -fmessage-length=0 -std=c++11\
		$(SRCDIR)/compute_score_host.cpp \
		$(SRCDIR)/MurmurHash2.c \
		$(SRCDIR)/main.cpp \
		-g -pg \
		-o ./host

host: $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.h
	g++ -D__USE_XOPEN2K8 -D__USE_XOPEN2K8 \
		-I$(SRCDIR) \
		-O3 -Wall -fmessage-length=0 -std=c++11\
		$(SRCDIR)/compute_score_host.cpp \
		$(SRCDIR)/MurmurHash2.c \
		$(SRCDIR)/main.cpp \
		-o ./host


clean:
	rm -rf temp_dir log_dir report_dir *log host runOnfpga* *.csv *summary .run .Xil vitis* *jou xilinx* gpofresult.txt gmon.out
