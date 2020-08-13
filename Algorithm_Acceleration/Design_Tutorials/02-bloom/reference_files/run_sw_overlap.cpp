#include <vector>
#include <cstdio>
#include <ctime>

#include "xcl2.hpp"
#include "sizes.h"
#include "common.h"

using namespace std;
using namespace std::chrono;

string kernel_name = "runOnfpga";
const char* kernel_name_charptr = kernel_name.c_str();
unsigned int bloom_filter_size = 1L<<bloom_size;
unsigned int profile_size = 1L<<24;
unsigned size_per_iter_const=512*1024;
unsigned size_per_iter;



void runOnFPGA(	
	unsigned int*  doc_sizes,
	unsigned int*  input_doc_words,
	unsigned int*  bloom_filter,
	unsigned long* profile_weights,
	unsigned long* profile_score,
	unsigned int   total_num_docs, 
	unsigned int   total_doc_size,
	int            num_iter)
{
	if ((total_doc_size/num_iter)%64!=0) {
		printf("--------------------------------------------------------------------\n");
		printf("ERROR: The number of word per iterations must be a multiple of 64\n");
		printf("       Total words = %d, Number of iterations = %d, Word per iterations = %d\n", total_doc_size, num_iter, total_doc_size/num_iter);
		printf("       Skipping FPGA kernel execution\n");
		exit(-1);
	}

	// Boilerplate code to load the FPGA binary, create the kernel and command queue
	vector<cl::Device> devices = xcl::get_xil_devices();
	cl::Device device = devices[0];
	cl::Context context(device);
	cl::CommandQueue q(context,device, CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE );

	string run_type = xcl::is_emulation()?(xcl::is_hw_emulation()?"hw_emu":"sw_emu"):"hw";
	string binary_file = kernel_name + "_" + run_type + ".xclbin";
	cl::Program::Binaries bins = xcl::import_binary_file(binary_file);
	cl::Program program(context, devices, bins);
	cl::Kernel kernel(program,kernel_name_charptr,NULL);

	unsigned int total_size = total_doc_size;
	unsigned char* output_inh_flags = (unsigned char*)aligned_alloc(4096, total_size*sizeof(char));
	bool load_filter = true;

	// Create buffers
	cl::Buffer buffer_bloom_filter(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, bloom_filter_size*sizeof(uint),bloom_filter);
	cl::Buffer buffer_input_doc_words(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, total_size*sizeof(uint),input_doc_words);
	cl::Buffer buffer_output_inh_flags(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, total_size*sizeof(char),output_inh_flags);

	// Set buffer kernel arguments (needed to migrate the buffers in the correct memory) 
	kernel.setArg(0, buffer_output_inh_flags);
	kernel.setArg(1, buffer_input_doc_words);
	kernel.setArg(2, buffer_bloom_filter);

	// Make buffers resident in the device
	q.enqueueMigrateMemObjects({buffer_bloom_filter, buffer_input_doc_words, buffer_output_inh_flags}, CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED);

	// Specify size of sub-buffers for each iteration 
	unsigned subbuf_doc_sz = total_doc_size/num_iter;
	unsigned subbuf_inh_sz = total_doc_size/num_iter;

        // Declare sub-buffer regions which specify offset and size for each iteration
	cl_buffer_region subbuf_inh_info[num_iter];
	cl_buffer_region subbuf_doc_info[num_iter];

        // Declare sub-buffers for each iteration
	cl::Buffer subbuf_inh_flags[num_iter];
	cl::Buffer subbuf_doc_words[num_iter];

        // Define sub-buffers from buffers based on sub-buffer regions
	for (int i=0; i<num_iter; i++) {
		subbuf_inh_info[i]={i*subbuf_inh_sz*sizeof(char), subbuf_inh_sz*sizeof(char)};
		subbuf_doc_info[i]={i*subbuf_doc_sz*sizeof(uint), subbuf_doc_sz*sizeof(uint)};
		subbuf_inh_flags[i] = buffer_output_inh_flags.createSubBuffer(CL_MEM_WRITE_ONLY, CL_BUFFER_CREATE_TYPE_REGION, &subbuf_inh_info[i]);
		subbuf_doc_words[i] = buffer_input_doc_words.createSubBuffer (CL_MEM_READ_ONLY,  CL_BUFFER_CREATE_TYPE_REGION, &subbuf_doc_info[i]);
	}

	printf("\n");
    double mbytes_total  = (double)(total_doc_size * sizeof(int)) / (double)(1000*1000);
    double mbytes_block  = mbytes_total / num_iter;
    printf(" Processing %.3f MBytes of data\n", mbytes_total);
    if (num_iter>1) {
    printf(" Splitting data in %d sub-buffers of %.3f MBytes for FPGA processing\n", num_iter, mbytes_block);
    }

    // Create Events to co-ordinate read,compute and write for each iteration 
	vector<cl::Event> wordWait;
	vector<cl::Event> krnlWait;
	vector<cl::Event> flagWait;

    printf("--------------------------------------------------------------------\n");

	chrono::high_resolution_clock::time_point t1, t2;
	t1 = chrono::high_resolution_clock::now();

	// Set Kernel arguments and load bloom filter coefficients
	cl::Event buffDone, krnlDone;
	total_size = 0;
	load_filter = true;
	kernel.setArg(3, total_size);
	kernel.setArg(4, load_filter);
	q.enqueueMigrateMemObjects({buffer_bloom_filter}, 0, NULL, &buffDone);
	wordWait.push_back(buffDone);
	q.enqueueTask(kernel, &wordWait, &krnlDone);
	krnlWait.push_back(krnlDone);
 
        // Set Kernel arguments. Read,enqueue the kernel and write for each iteration
	for (int i=0; i<num_iter; i++) 
	{
		cl::Event buffDone, krnlDone, flagDone;
		total_size = subbuf_doc_info[i].size / sizeof(uint);
		load_filter = false;
		kernel.setArg(0, subbuf_inh_flags[i]);
		kernel.setArg(1, subbuf_doc_words[i]);
		kernel.setArg(3, total_size);
		kernel.setArg(4, load_filter);
		q.enqueueMigrateMemObjects({subbuf_doc_words[i]}, 0, &wordWait, &buffDone); 
		wordWait.push_back(buffDone);
		q.enqueueTask(kernel, &wordWait, &krnlDone);
		krnlWait.push_back(krnlDone);
		q.enqueueMigrateMemObjects({subbuf_inh_flags[i]}, CL_MIGRATE_MEM_OBJECT_HOST, &krnlWait, &flagDone);
		flagWait.push_back(flagDone);
	}


	// Create variables to keep track of number of words needed by CPU to compute score and number of words processed by FPGA such that CPU processing can overlap with FPGA
        unsigned int curr_entry;
	unsigned char inh_flags;
	unsigned int  available = 0;
	unsigned int  needed = 0;
	unsigned int  iter = 0;
			
	for(unsigned int doc=0, n=0; doc<total_num_docs;doc++) 
	{
		unsigned long ans = 0;
		unsigned int size = doc_sizes[doc];
		
                // Calculate size by needed by CPU for processing next document score
		needed += size;
		if (needed > available) {
			flagWait[iter].wait();
			available += subbuf_doc_info[iter].size / sizeof(uint);
			iter++;
		}
 
	        // Check if flgas processed by FPGA is greater than needed by CPU. Else, block CPU
                // Update the number of available words and sub-buffer count(iter)
		for (unsigned i = 0; i < size ; i++, n++)
		  { 
			curr_entry = input_doc_words[n];
			inh_flags  = output_inh_flags[n];

			if (inh_flags) 
			{
				unsigned frequency = curr_entry & 0x00ff;
				unsigned word_id = curr_entry >> 8;

				ans += profile_weights[word_id] * (unsigned long)frequency;
			}
		 }
		profile_score[doc] = ans;
	}

	t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> perf_all_sec  = chrono::duration_cast<duration<double>>(t2-t1);

    cl_ulong f1 = 0;
    cl_ulong f2 = 0;
    wordWait.front().getProfilingInfo(CL_PROFILING_COMMAND_QUEUED, &f1);
    flagWait.back().getProfilingInfo(CL_PROFILING_COMMAND_END, &f2);
    double perf_hw_ms = (f2 - f1)/1000000.0;

    if (xcl::is_emulation()) {
    	if (xcl::is_hw_emulation()) {
		    printf(" Emulated FPGA accelerated version  | run 'vitis_analyzer xclbin.run_summary' for performance estimates");
    	} else {
		    printf(" Emulated FPGA accelerated version  | (performance not relevant in SW emulation)");
		}
    } else {
		    printf(" Executed FPGA accelerated version  | %10.4f ms   ( FPGA %.3f ms )", 1000*perf_all_sec.count(), perf_hw_ms);    	
    }
	printf("\n");
}

