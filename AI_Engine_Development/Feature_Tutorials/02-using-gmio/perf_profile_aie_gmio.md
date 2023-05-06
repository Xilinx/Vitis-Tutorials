<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# AI Engine GMIO Performance Profile

AI Engine tools support mapping the GMIO port to the tile DMA one-to-one. It does not support mapping multiple GMIO ports to one tile DMA channel. There is a limit on the number of GMIO ports supported for a given device. For example, the XCVC1902 device on the VCK190 board
has 16 AI Engine to NoC master unit (NMU) in total. For each AI Engine to NMU, it supports two MM2S and two S2MM channels. Hence there can be a maximum of 32 AI Engine GMIO inputs, 32 AI Engine GMIO outputs supported, but note that it can be further limited by the existing hardware platform.

In this example, 4 AI Engine GMIO inputs, 4 AI Engine GMIO outputs are utilized in the graph and the performance for the graph is profiled through various ways. Then you will learn about the usage of GMIO for data transfer.  

## Design Introduction

This design has a graph that has 4 AI Engine kernels. Each kernel has one input and one output. Thus, 4 AI Engine GMIO inputs and 4 AI Engine GMIO outputs are connected to the graph.

Change the working directory to `perf_profile_aie_gmio`. Take a look at the graph code in `aie/graph.h`.

	static const int col[8]={2,6,10,18,26,34,42,46};
	static const int NUM=4;
	
	class topgraph: public adf::graph
	{
	public:
		adf::kernel k[NUM];
		adf::input_gmio gmioIn[NUM];	
		adf::output_gmio gmioOut[NUM];
		
		topgraph(){
			for(int i=0;i<NUM;i++){
				k[i] = adf::kernel::create(vec_incr);
				adf::source(k[i]) = "vec_incr.cc";
				adf::runtime<adf::ratio>(k[i])= 1;
				gmioIn[i]=adf::input_gmio::create("gmioIn"+std::to_string(i),/*size_t burst_length*/256,/*size_t bandwidth*/100);
				gmioOut[i]=adf::output_gmio::create("gmioOut"+std::to_string(i),/*size_t burst_length*/256,/*size_t bandwidth*/100);
				adf::connect<>(gmioIn[i].out[0], k[i].in[0]);	
				adf::connect<>(k[i].out[0], gmioOut[i].in[0]);
	
				adf::location<adf::kernel>(k[i])=adf::tile(col[i],0);
				location<GMIO>(gmioIn[i]) = location<kernel>(k[i]) + relative_offset({.col_offset=0});	
				location<GMIO>(gmioOut[i]) = location<kernel>(k[i]) + relative_offset({.col_offset=1});
			}
		}
	};

In the previous code, there are location constraints `adf::location` for each kernel. And there relative constraints for GMIO inputs and GMIO outputs. That's to place GMIO ports to different columns, and performance counters will not run out when profiling all ports with event API at the same time.

Next, examine the kernel code `aie/vec_incr.cc`. It adds each int32 input by one and additionally outputs the cycle counter of the AI Engine tile. Due to the later introduction, this counter can be used to calculate the system throughput.

	using namespace adf;
	void vec_incr(input_buffer<int32,extents<256>>& __restrict data,output_buffer<int32,extents<258>>& __restrict out){
		auto inIter=aie::begin_vector<16>(data);
		auto outIter=aie::begin_vector<16>(out);
		aie::vector<int32,16> vec1=aie::broadcast<int32>(1);
		for(int i=0;i<16;i++)
		chess_prepare_for_pipelining
		{
			aie::vector<int32,16> vdata=*inIter++;
			aie::vector<int32,16> vresult=aie::add(vdata,vec1);
			*outIter++=vresult;
		}
		aie::tile tile=aie::tile::current();
		unsigned long long time=tile.cycles();//cycle counter of the AI Engine tile
		decltype(aie::begin(out)) p=*(decltype(aie::begin(out))*)&outIter;
		*p++=time&0xffffffff;
		*p++=(time>>32)&0xffffffff;
	}

Next, examine the host code `sw/host.cpp`. The concepts introduced in [AIE GMIO Programming Model](./single_aie_gmio.md) apply here. This section explains new concepts and how performance profiling is done. Some constants defined in the code are as follows:

	const int NUM=4;
	int ITERATION=8192;	
	char* emu_mode = getenv("XCL_EMULATION_MODE");
    if (emu_mode != nullptr) {
		ITERATION=4;
	}
    const int BLOCK_SIZE_in_Bytes=1024*ITERATION;
    const int BLOCK_SIZE_out_Bytes=1032*ITERATION;

If it is for hardware flow, `ITERATION` is 8192 otherwise, it is 4. This is to make sure that the AI Engine simulator can finish in a short amount of time.

In the main function, the PS code is going to profile `NUM` GMIO inputs and outputs, where `NUM` is 4. Non-blocking GMIO APIs (`GMIO::gm2aie_nb` and `GMIO::aie2gm_nb`) are used for GMIO transactions, and `GMIO::wait` is used for output data synchronization. 

	//Pre-processing
	......

	//start graph and GMIO output ports first
	gr.run(ITERATION);
	for(int i=0;i<NUM;i++){
		gr.gmioOut[i].aie2gm_nb(doutArray[i], BLOCK_SIZE_out_Bytes);
	}

	//Profile starts here
	......

	//start GMIO inputs and wait for GMIO outputs to complete
	for(int i=0;i<NUM;i++){
		gr.gmioIn[i].gm2aie_nb(dinArray[i], BLOCK_SIZE_in_Bytes);
	}
	for(int i=0;i<NUM;i++){
		gr.gmioOut[i].wait();
	}

	//Profile ends here
	......

	//check output correctness 
	......

## Performance Profiling Methods

In this example, we will introduce some methods for profiling the design. The profiling code is in `sw/host.cpp`.

__Note:__ This tutorial assumes that AI Engine runs at 1.25 GHz.

1. Profile by C++ class API

The code to use C++ class API is common for Linux system for various platforms. The `Timer` is defined as follows:

    class Timer {
      std::chrono::high_resolution_clock::time_point mTimeStart;
      public:
        Timer() { reset(); }
        long long stop() {
          std::chrono::high_resolution_clock::time_point timeEnd =
              std::chrono::high_resolution_clock::now();
          return std::chrono::duration_cast<std::chrono::microseconds>(timeEnd -
                                                                      mTimeStart)
              .count();
        }
        void reset() { mTimeStart = std::chrono::high_resolution_clock::now(); }
    };

The code to start profiling is as follows:

    Timer timer;

The code to end profiling and calculate performance is as follows:

    double timer_stop=timer.stop();
    double throughput=(BLOCK_SIZE_in_Bytes+BLOCK_SIZE_out_Bytes)*NUM/timer_stop;
    std::cout<<"Throughput (by timer GMIO in num="<<num<<",out num="<<num<<"):\t"<<throughput<<"M Bytes/s"<<std::endl;

The code is guarded by macro `__TIMER__`. To use this method of profiling, define `__TIMER__` for g++ cross compiler in `sw/Makefile`:

	CXXFLAGS += -std=c++17 -D__TIMER__ ......

To run it in hardware, use the following make command to build the hardware image:

    make package TARGET=hw

After the package is done, run the following commands in the Linux prompt after booting Linux from an SD card (use petalinux/petalinux to login):

    cd /run/media/mmcblk0p1
    ./host.exe a.xclbin

The output in hardware is similar as follows:

	Throughput (by timer GMIO in num=4,out num=4):9882.79M Bytes/s
	
2. Profile by AI Engine cycles got from AI Engine kernels

In this design, the AI Engine cycles output at the end of each iteration. Each iteration produces 256 int32 data, plus a long long AI Engine cycle counter number. The very beginning cycle and the last cycle of all the AI Engine kernels to be profiled are recorded because multiple AI Engine kernels start at different cycles though they are enabled by the same `graph::run`. Thus, the system throughput for all the kernels can be calculated.

Note that there is some gap between the actual performance and the calculated number because there are some data transfers before and after the recorded cycles. 

The code to get AI Engine cycles and calculate the system throughput is as follows:

	unsigned long long start[NUM];
	unsigned long long end[NUM];
	unsigned long long very_beginning=0xFFFFFFFFFFFFFFFF;
	unsigned long long the_last=0;
	for(int i=0;i<NUM;i++){
		start[i]=*(unsigned long long*)&doutArray[i][256];
		end[i]=*(unsigned long long*)&doutArray[i][BLOCK_SIZE_out_Bytes/sizeof(int)-2];
		if(start[i]<very_beginning){
			very_beginning=start[i];
		}
		if(end[i]>the_last){
			the_last=end[i];
		}
	}
	std::cout<<"Throughput (by AIE kernel cycles in="<<NUM<<",out="<<NUM<<") ="<<(double)(BLOCK_SIZE_in_Bytes+BLOCK_SIZE_out_Bytes)*NUM/((double)(the_last-very_beginning)*0.8)*1000<<"M Bytes/s"<<std::endl;

The code is guarded by macro `__AIE_CYCLES__`. To use this method of profiling, define `__AIE_CYCLES__` for g++ cross compiler in `sw/Makefile`:

    CXXFLAGS += -std=c++17 -D__AIE_CYCLES__ ......

The commands to build and run in hardware are the same as previously shown. The output in hardware is similar as follows:

	Throughput (by AIE kernel cycles in=4,out=4) =10561.8M Bytes/s
	
3. Profile by event API

The AI Engine has hardware performance counters and can be configured to count hardware events for measuring performance metrics. The API used in this example is to profile graph throughput regarding the specific GMIO ports. There may be confliction when multiple GMIO ports are used for event API because of the restriction that performance counter is shared between GMIO ports that access the same AI Engine-PL interface column. Thus, all GMIO ports are constrained to different columns to avoid such conflictions.

The code to start profiling is as follows:

	event::handle handle[NUM];
	for(int i=0;i<NUM;i++){
		handle[i] = event::start_profiling(gr.gmioOut[i], event::io_stream_start_to_bytes_transferred_cycles, BLOCK_SIZE_out_Bytes);
	}
	
The code to end profling and calculate performance is as follows:

	long long cycle_count[NUM];
	long long total_cycle_count=0;
	for(int i=0;i<NUM;i++){
		cycle_count[i] = event::read_profiling(handle[i]);
		event::stop_profiling(handle[i]);
		if(cycle_count[i]>total_cycle_count){
			total_cycle_count=cycle_count[i];
		}
	}
	double bandwidth = (double)(BLOCK_SIZE_in_Bytes+BLOCK_SIZE_out_Bytes)*NUM / ((double)total_cycle_count*0.8) *1000; //byte per second
	std::cout<<"Throughput (by event API) bandwidth="<<bandwidth<<"M Bytes/s"<<std::endl;
	
In this example, `event::start_profiling is` called to configure the AI Engine to count the clock cycles from the stream start event to the event that indicates `BLOCK_SIZE_out_Bytes` bytes have been transferred, assuming that the stream stops right after the specified number of bytes are transferred.

For detailed usage about event API, refer to the *Versal ACAP AI Engine Programming Environment User Guide* ([UG1076]).

The code is guarded by macro `__USE_EVENT_PROFILE__`. To use this method of profiling, define `__USE_EVENT_PROFILE__` for g++ cross compiler in `sw/Makefile`:

    CXXFLAGS += -std=c++17 -D__USE_EVENT_PROFILE__ ......

The commands to build and run in hardware are the same as previously shown. The output in hardware is similar as follows:

	Throughput (by event API) bandwidth=10571.6M Bytes/s
	
### Conclusion

In this tutorial, you learned about:

* Programming model for AI Engine GMIO
* Ways to profile system performance.


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
