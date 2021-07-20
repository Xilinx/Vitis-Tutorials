#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <sys/mman.h>
#include <unistd.h>
extern "C" {
    #include <xaiengine.h>
}
#include <fcntl.h>
#include <sys/stat.h>

#include "tx_chain.cpp"
#include "helper_functions.h"

# include "xrt/experimental/xrt_aie.h"
# include "xrt/experimental/xrt_kernel.h"
#include "xrt/experimental/xrt_bo.h"

#include <adf.h>

using namespace std;
//using namespace cardano;
#include "adf/adf_api/XRTConfig.h"


#define LEN			0x1000U
#define BURSTLEN		8U
#define raw_printf(format, ...) printf(format, ##__VA_ARGS__)
#define raw_perror(format, ...) fprintf(stderr, format, ##__VA_ARGS__)
#define LPERROR(format, ...) raw_perror("ERROR: PLIO Static Latency> " format, ##__VA_ARGS__)

#define NUM_INPUT_SAMPLES 10240
#define NUM_OUTPUT_SAMPLES 65536
#define TX_CHAIN_PERF


void config_dmas() {
    printf("Configuring mm2s_0 \n");
    system("devmem 0xA4010010 32 0x70000000");
    system("devmem 0xA4010014 32 0x00000000");
    system("devmem 0xA401001C 32 0x00002800");
    system("devmem 0xA4010000 32 0x81");

    printf("Configuring mm2s_1 \n");
    system("devmem 0xA4020010 32 0x71000000");
    system("devmem 0xA4020014 32 0x00000000");
    system("devmem 0xA402001C 32 0x00002800");
    system("devmem 0xA4020000 32 0x81");

    printf("Configuring s2mm_0 \n");
    system("devmem 0xA4030010 32 0x73000000");
    system("devmem 0xA4030014 32 0x00000000");
    system("devmem 0xA403001C 32 0x00003AC4");
    system("devmem 0xA4030000 32 0x81");
    /////////////////////////////////////
   }

static std::vector<char>
load_xclbin(xrtDeviceHandle device, const std::string& fnm)
{
   if (fnm.empty())
      throw std::runtime_error("No xclbin speified");

   // load bit stream
   std::ifstream stream(fnm);
   stream.seekg(0,stream.end);
   size_t size = stream.tellg();
   stream.seekg(0,stream.beg);

   std::vector<char> header(size);
   stream.read(header.data(),size);

   auto top = reinterpret_cast<const axlf*>(header.data());
   if (xrtDeviceLoadXclbin(device, top))
      throw std::runtime_error("Bitstream download failed");

   return header;
}

int main(int argc, char** argv) {
	#define NUM_ELEMS 1024

    if (argc < 2) {
      std::cout << "Usage: tx_chain <xclbin>\n";
      return 1;
    }

    auto dhdl = xrtDeviceOpen(0);
    auto xclbin = load_xclbin(dhdl, argv[1]);
    auto top = reinterpret_cast<const axlf*>(xclbin.data());
    adf::registerXRT(dhdl, top->m_header.uuid);


    
	int32_t coeffs1[NUM_ELEMS];
	if (! readIntegersFromFile("data/coefs_4c2l_ps01.txt", coeffs1, NUM_ELEMS)) {
		cerr << "main(): Error reading input data file." << endl;
	    return 1;
	}
	int32_t coeffs2[NUM_ELEMS];
	if (! readIntegersFromFile("data/coefs_4c2l_ps23.txt", coeffs2, NUM_ELEMS)) {
		cerr << "main(): Error reading input data file." << endl;
		return 1;
	}


    unsigned int sleep = 1000;
    usleep(sleep);
    // Store the input hex data into *lte0 and *lte1

    printf("Reset the aie array \n");
//	system("./versal_run/reset-board.sh");
    usleep(1000);

    // Start tx_chain
    printf("Initializing the tx_chain0...\n");

    auto graphHandle = xrtGraphOpen(dhdl, top->m_header.uuid, "tx_chain0");
    if (!graphHandle) {
        throw std::runtime_error("Unable to open graph handle");
        return 1;
    }

 

    printf("Running the tx_chain0...\n");

   int ret = xrtGraphReset(graphHandle);
   ret = xrtGraphRun(graphHandle,-1);
   if (ret) {
      throw std::runtime_error("Unable to run graph");
       return 1;
   }



    usleep(sleep);

    // Update the coefs of tx_chain
    printf("Updating the tx_chain for coeffs1 and coeffs2...\n");

       int ret1, ret2; 
   std::string port_c2l_0;
   std::string port_c2l_1;

  	port_c2l_0 = "tx_chain0.dpd_filter.c2l[0].in[0]"; 
       port_c2l_1 = "tx_chain0.dpd_filter.c2l[1].in[0]";	

   ret1 = xrtGraphUpdateRTP(graphHandle, port_c2l_0.c_str(), reinterpret_cast<const char *>(coeffs1), 4096);
   printf("ret1:%b\n",ret1);

    if (ret1) {
   	throw std::runtime_error("Unable to update rtp");
       return 1;
   }

   ret2 = xrtGraphUpdateRTP(graphHandle, port_c2l_1.c_str(), reinterpret_cast<const char *>(coeffs2), 4096);
   printf("ret2:%b\n",ret2);

   if (ret2) {
   	throw std::runtime_error("Unable to update rtp");
       return 1;
   }



    config_dmas();

    
    // initialize event counters
#ifdef TX_CHAIN_PERF
     long long throughput_in[1], throughput_out[1] = {0};
     long long count_start_in[1], count_end_in[1] = {0};
     long long count_start_out[1], count_end_out[1] = {0};
     
     event::handle handle_in[1], handle_out[1];
     handle_out[0] = event::start_profiling(*out0, event::io_stream_running_event_count);
     handle_in[0] = event::start_profiling(*in1, event::io_stream_running_event_count);
     int tx_chain_num = 0;
#endif

    // Run the data through tx_chain
    printf("Start mm2s and s2mm kernels...\n");

#ifdef TX_CHAIN_PERF
     while (1){
     	//int j = 0;
	//while(j < 1) {
    	//for (tx_chain_num = 0; tx_chain_num < 1 * NUM_OF_ANT; tx_chain_num++) {
	   	count_start_out[0]  = event::read_profiling(handle_out[0]);
		count_start_in[0]   = event::read_profiling(handle_in[0]);
	        usleep(500000);
		count_end_out[0]     = event::read_profiling(handle_out[0]);
		count_end_in[0]    = event::read_profiling(handle_in[0]);
		if (count_end_in[0] > count_start_in[0])
			throughput_in[0] = (count_end_in[0] - count_start_in[0])<<1;
		else
			throughput_in[0] = (count_end_in[0] - count_start_in[0] + 4294967296)<<1;
	
		if (count_end_out[0] > count_start_out[0])
			throughput_out[0] = (count_end_out[0] - count_start_out[0])<<1;
		else
			throughput_out[0] = (count_end_out[0] - count_start_out[0] + 4294967296)<<1;
	
		printf("TX Chain %d :  DPD througput:%llu \t LTE0 throughput:%llu \t\n", tx_chain_num, throughput_out[0], throughput_in[0]);
	//	j = j + 1;
	//}
    }
#endif

       return 0;
}
