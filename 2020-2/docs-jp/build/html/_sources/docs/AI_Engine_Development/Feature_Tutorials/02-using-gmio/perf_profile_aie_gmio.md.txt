<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

<<<<<<< HEAD
## AI Engine GMIO Performance Profile
=======
# AI Engine GMIO Performance Profile

AI Engine tools support mapping the GMIO port to the tile DMA one-to-one. It does not support mapping multiple GMIO ports to one tile DMA channel. There is a limit on the number of GMIO ports supported for a given device. For example, the XCVC1902 device on the VCK190 board
has 16 AI Engine to NoC master unit (NMU) in total. For each AI Engine to NMU, it supports two MM2S and two S2MM channels. Hence there can be a maximum of 32 AI Engine GMIO inputs, 32 AI Engine GMIO outputs supported, but note that it can be further limited by the existing hardware platform.

In this example, 32 AI Engine GMIO inputs, 32 AI Engine GMIO outputs are utilized in the graph and the performance from one input and one output to 32 inputs and 32 outputs through various ways is profiled. Then you will learn about the NOC bandwidth and the advantages and disadvantages of choosing GMIO for data transfer.  

## Design Introduction

This design has a graph that has 32 AI Engine kernels. Each kernel has one input and one output. Thus, 32 AI Engine GMIO inputs and 32 AI Engine GMIO outputs are connected to the graph.

Change the working directory to `perf_profile_aie_gmio`. Take a look at the graph code in `aie/graph.h`.

    static const int col[32]={6,13,14,45,18,42,4,30,48,49,9,16,29,39,40,31,2,3,46,0,43,27,41,26,11,17,47,1,19,10,34,7};

	class mygraph: public adf::graph
	{
	private:
	  adf::kernel k[32];

	public:
	  adf::input_gmio gmioIn[32];
	  adf::output_gmio gmioOut[32];

	  mygraph()
	  {
		for(int i=0;i<32;i++){
			gmioIn[i]=adf::input_gmio::create("gmioIn"+std::to_string(i),/*size_t burst_length*/256,/*size_t bandwidth*/100);
			gmioOut[i]=adf::output_gmio::create("gmioOut"+std::to_string(i),/*size_t burst_length*/256,/*size_t bandwidth*/100);
			k[i] = adf::kernel::create(vec_incr);
			adf::connect<adf::window<1024>>(gmioIn[i].out[0], k[i].in[0]);
			adf::connect<adf::window<1032>>(k[i].out[0], gmioOut[i].in[0]);
			adf::source(k[i]) = "vec_incr.cc";
			adf::runtime<adf::ratio>(k[i])= 1;
			adf::location<adf::kernel>(k[i])=adf::tile(col[i],0);
		}
	  };
	};

In the previous code, there are location constraints `adf::location` for each kernel. This is to save time for `aiecompiler`. Note that each kernel has an input window size of 1024 bytes and output window size of 1032 bytes.

Next, examine the kernel code `aie/vec_incr.cc`. It adds each int32 input by one and additionally outputs the cycle counter of the AI Engine tile. Due to the later introduction, this counter can be used to calculate the system throughput.

	#include <aie_api/aie.hpp>
	#include <aie_api/aie_adf.hpp>
	#include <aie_api/utils.hpp>

	void vec_incr(input_window<int32>* data,output_window<int32>* out){
		aie::vector<int32,16> vec1=aie::broadcast<int32>(1);
		for(int i=0;i<16;i++)
		chess_prepare_for_pipelining
		chess_loop_range(4,)
		{
			aie::vector<int32,16> vdata=window_readincr_v<16>(data);
			aie::vector<int32,16> vresult=aie::add(vdata,vec1);
			window_writeincr(out,vresult);
		}
		aie::tile tile=aie::tile::current();
		unsigned long long time=tile.cycles();//cycle counter of the AI Engine tile
		window_writeincr(out,time);
	}

Next, examine the host code `aie/graph.cpp`. The concepts introduced in [AIE GMIO Programming Model](./single_aie_gmio.md) apply here. This section explains new concepts and how performance profiling is done. Some constants defined in the code are as follows:

    #if !defined(__AIESIM__) && !defined(__X86SIM__) && !defined(__ADF_FRONTEND__)
    const int ITERATION=512;
    #else
    const int ITERATION=4;
    #endif
    const int BLOCK_SIZE_in_Bytes=1024*ITERATION;
    const int BLOCK_SIZE_out_Bytes=1032*ITERATION;

If it is for hardware flow, `ITERATION` is 512 otherwise, it is 4. This is to make sure that the AI Engine simulator can finish in a short amount of time.

In the main function, the PS code is going to profile `num` GMIO inputs and outputs, and `num` is from 1, 2, 4, to 32. Non-blocking GMIO APIs (`GMIO::gm2aie_nb` and `GMIO::aie2gm_nb`) are used for GMIO transactions, and `GMIO::wait` is used for output data synchronization. Only when the input and output data are transferred for the kernel, can the kernel be finished. This is because the graph is started for all the AI Engine kernels, but only some of the kernels are profiled. After the code for profiling, the remaining kernels are flushed by transferring data to and from the remaining AI Engine kernels.

    for(int num=1;num<=32;num*=2){
      //Pre-processing
      for(int i=0;i<32;i++){
        for(int j=0;j<BLOCK_SIZE_in_Bytes/sizeof(int);j++){
         dinArray[i][j]=j+num;
        }
       }
       gr.run(ITERATION);

       //Profile starts here
       for(int i=0;i<num;i++){
       	gr.gmioIn[i].gm2aie_nb(dinArray[i], BLOCK_SIZE_in_Bytes);
        gr.gmioOut[i].aie2gm_nb(doutArray[i], BLOCK_SIZE_out_Bytes);
       }
       for(int i=0;i<num;i++){
        gr.gmioOut[i].wait();
       }
       //Profile ends here

       //check output correctness
       for(int i=0;i<num;i++){
            for(int j=0;j<BLOCK_SIZE_out_Bytes/sizeof(int);j++){
              if(j%258!=256 && j%258!=257 && doutArray[i][j]!=j+num+1-j/258*2){
                 std::cout<<"ERROR:dout["<<i<<"]["<<j<<"]="<<doutArray[i][j]<<std::endl;
                 error++;
               break;
              }
           }
        }

      //flush remain stalling kernels
      for(int i=num;i<32;i++){
       gr.gmioIn[i].gm2aie_nb(dinArray[i], BLOCK_SIZE_in_Bytes);
       gr.gmioOut[i].aie2gm_nb(doutArray[i], BLOCK_SIZE_out_Bytes);
      }
      gr.wait();
    }

<<<<<<< HEAD
### Performance Profiling Methods
In this example, some methods for profiling the design are introduced. The code to be profiled is in `aie/graph.cpp`:
=======
## Performance Profiling Methods

In this example, we will introduce some methods for profiling the design. The code to be profiled is in `aie/graph.cpp`:
>>>>>>> d8d61191... Updated headers and footers, fixed image references (#33)

    //Profile starts here
    for(int i=0;i<num;i++){
      gr.gmioIn[i].gm2aie_nb(dinArray[i], BLOCK_SIZE_in_Bytes);
      gr.gmioOut[i].aie2gm_nb(doutArray[i], BLOCK_SIZE_out_Bytes);
    }
    for(int i=0;i<num;i++){
      gr.gmioOut[i].wait();
    }
    //Profile ends here

__Note:__ This tutorial assumes that AI Engine runs at 1 GHz.

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
    double throughput=(BLOCK_SIZE_in_Bytes+BLOCK_SIZE_out_Bytes)*num/timer_stop*1000000/1024/1024;
    std::cout<<"Throughput (by timer GMIO in num="<<num<<",out num="<<num<<"):\t"<<throughput<<"M Bytes/s"<<std::endl;

The code is guarded by macro `__TIMER__`. To use this method of profiling, define `__TIMER__` for g++ cross compiler in `sw/Makefile`:

    CXXFLAGS += -std=c++14 -D__TIMER__ -I$(XILINX_HLS)/include/ -I${SYSROOT}/usr/include/xrt/ -O0 -g -Wall -c -fmessage-length=0 --sysroot=${SYSROOT} -I${XILINX_VITIS}/aietools/include ${HOST_INC}

To run it in hardware, use the following make command to build the hardware image:

    make package TARGET=hw

After the package is done, run the following commands in the Linux prompt after booting Linux from an SD card:

    export XILINX_XRT=/usr
    cd /run/media/mmcblk0p1
    ./host.exe a.xclbin

The output in hardware is as follows:

    GMIO::malloc completed
    Throughput (by timer GMIO in num=1,out num=1):5076.64M Bytes/s
    Throughput (by timer GMIO in num=2,out num=2):8335.5M Bytes/s
    Throughput (by timer GMIO in num=4,out num=4):9543.97M Bytes/s
    Throughput (by timer GMIO in num=8,out num=8):9717.18M Bytes/s
    Throughput (by timer GMIO in num=16,out num=16):10154.1M Bytes/s
    Throughput (by timer GMIO in num=32,out num=32):10246.8M Bytes/s
    AIE GMIO PASSED!
    GMIO::free completed
    PASS!

2. Profile by AI Engine cycles got from AI Engine kernels

In this design, the AI Engine cycles output at the end of each iteration. Each iteration produces 256 int32 data, plus a long long AI Engine cycle counter number. The very beginning cycle and the last cycle of all the AI Engine kernels to be profiled are recorded because multiple AI Engine kernels start at different cycles though they are enabled by the same `graph::run`. Thus, the system throughput for all the kernels can be calculated.

Note that there is some gap between the actual performance and the calculated number because there is already some data transfer before the recorded starting cycle. However, the overhead is negligible when the total iteration number is high, which is 512 in this example.

The code to get AI Engine cycles and calculate the system throughput is as follows:

    long long start[32];
    long long end[32];
    long long very_beginning=0x0FFFFFFFFFFFFFFF;
    long long the_last=0;
    for(int i=0;i<num;i++){
      start[i]=*(long long*)&doutArray[i][256];
      end[i]=*(long long*)&doutArray[i][BLOCK_SIZE_out_Bytes/sizeof(int)-2];
      if(start[i]<very_beginning){
        very_beginning=start[i];
      }
      if(end[i]>the_last){
        the_last=end[i];
      }
    }
    std::cout<<"Throughput (by AIE kernel cycles in="<<num<<",out="<<num<<") ="<<(double)(BLOCK_SIZE_in_Bytes+BLOCK_SIZE_out_Bytes)*num/(double)(the_last-very_beginning)*1000<<"M Bytes/s"<<std::endl;

The code is guarded by macro `__AIE_CYCLES__`. To use this method of profiling, define `__AIE_CYCLES__` for g++ cross compiler in `sw/Makefile`:

    CXXFLAGS += -std=c++14 -D__AIE_CYCLES__ -I$(XILINX_HLS)/include/ -I${SYSROOT}/usr/include/xrt/ -O0 -g -Wall -c -fmessage-length=0 --sysroot=${SYSROOT} -I${XILINX_VITIS}/aietools/include ${HOST_INC}

The commands to build and run in hardware are the same as previously shown. The output in hardware is as follows:

    GMIO::malloc completed
    Throughput (by AIE kernel cycles in=1,out=1) =6545.68M Bytes/s
    Throughput (by AIE kernel cycles in=2,out=2) =10267.4M Bytes/s
    Throughput (by AIE kernel cycles in=4,out=4) =11036.8M Bytes/s
    Throughput (by AIE kernel cycles in=8,out=8) =10807.9M Bytes/s
    Throughput (by AIE kernel cycles in=16,out=16) =10958.2M Bytes/s
    Throughput (by AIE kernel cycles in=32,out=32) =10892.8M Bytes/s
    AIE GMIO PASSED!
    GMIO::free completed
    PASS!

3. Profile by event API

The AI Engine has hardware performance counters and can be configured to count hardware events for measuring performance metrics. The API used in this example is to profile graph throughput regarding the specific GMIO port. There may be confliction when multiple GMIO ports are used for event API because of the restriction that performance counter is shared between GMIO ports that access the same AI Engine-PL interface column. Thus, only one GMIO output is profiled to show this methodology.

The code to start profiling is as follows:

    std::cout<<"total input/output num="<<num<<std::endl;
    event::handle handle[32];
    for(int i=0;i<1;i++){
      handle[i] = event::start_profiling(gr.gmioOut[i], event::io_stream_start_to_bytes_transferred_cycles, BLOCK_SIZE_out_Bytes);
    }

The code to end profling and calculate performance is as follows:

    long long cycle_count[32];
    for(int i=0;i<1;i++){
      cycle_count[i] = event::read_profiling(handle[i]);
      event::stop_profiling(handle[i]);
    }
    for(int i=0;i<1;i++){
      double bandwidth = (double)BLOCK_SIZE_out_Bytes / ((double)cycle_count[i] ) *1000; //byte per second
      std::cout<<"Throughput (by event API) gmioOut["<<i<<"] bandwidth="<<bandwidth<<"M Bytes/s"<<std::endl;
    }

In this example, `event::start_profiling is` called to configure the AI Engine to count the clock cycles from the stream start event to the event that indicates `BLOCK_SIZE_out_Bytes` bytes have been transferred, assuming that the stream stops right after the specified number of bytes are transferred.

For detailed usage about event API, refer to the *Versal ACAP AI Engine Programming Environment User Guide* ([UG1076]).

The code is guarded by macro `__USE_EVENT_PROFILE__`. To use this method of profiling, define `__USE_EVENT_PROFILE__` for g++ cross compiler in `sw/Makefile`:

    CXXFLAGS += -std=c++14 -D__USE_EVENT_PROFILE__ -I$(XILINX_HLS)/include/ -I${SDKTARGETSYSROOT}/usr/include/xrt/ -O0 -g -Wall -c -fmessage-length=0 --sysroot=${SDKTARGETSYSROOT} -I${XILINX_VITIS}/aietools/include ${HOST_INC}

The commands to build and run in hardware are the same as previously shown. The output in hardware is as follows:

    GMIO::malloc completed
    total input/output num=1
    Throughput (by event API) gmioOut[0] bandwidth=3321.1M Bytes/s
    total input/output num=2
    Throughput (by event API) gmioOut[0] bandwidth=2737.72M Bytes/s
    total input/output num=4
    Throughput (by event API) gmioOut[0] bandwidth=1608.08M Bytes/s
    total input/output num=8
    Throughput (by event API) gmioOut[0] bandwidth=1044.15M Bytes/s
    total input/output num=16
    Throughput (by event API) gmioOut[0] bandwidth=972.539M Bytes/s
    total input/output num=32
    Throughput (by event API) gmioOut[0] bandwidth=989.08M Bytes/s
    AIE GMIO PASSED!
    GMIO::free completed
    PASS!

It is seen that when the used GMIO ports number increases, the performance for the specific GMIO port drops, indicating that the total system throughput is limited by NOC and DDR bandwidth.

### Conclusion

In this tutorial, you learned about:

* Programming model for AI Engine GMIO
* Ways to profile system performance and also, about the NOC and DDR bandwidth.


<br></br>

Licensed under the Apache License, Version 2.0 (the "License");

you may not use this file except in compliance with the License.

You may obtain a copy of the License at



    http://www.apache.org/licenses/LICENSE-2.0



Unless required by applicable law or agreed to in writing, software

distributed under the License is distributed on an "AS IS" BASIS,

WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and

limitations under the License.



<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020–2021 Xilinx</sup><br><sup>XD007</sup></br></p>
