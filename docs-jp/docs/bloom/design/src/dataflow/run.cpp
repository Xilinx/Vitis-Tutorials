#include"kernels.h"
#include"sizes.h"
#include<vector>
#include<cstdio>
#include<ctime>
#include "xcl2.hpp"
using namespace std;
using namespace std::chrono;

string kernel_name = "runOnfpga";
const char* kernel_name_charptr = kernel_name.c_str();
unsigned int bloom_filter_size = 1L<<bloom_size;
unsigned int profile_size = 1L<<24;
unsigned size_per_iter_const=512*1024;
unsigned size_per_iter;

void run (uint* starting_doc_id,uint* doc_sizes,uint* input_doc_words,uint* bloom_filter,ulong* profile_weights,ulong* fpga_profileScore,unsigned int total_num_docs, unsigned int total_doc_size_1) {

vector<cl::Device> devices = xcl::get_xil_devices();
cl::Device device = devices[0];
cl::Context context(device);
cl::CommandQueue q(context,device,CL_QUEUE_PROFILING_ENABLE );

string run_type = xcl::is_emulation()?(xcl::is_hw_emulation()?"hw_emu":"sw_emu"):"hw";
string binary_file = kernel_name + "_" + run_type + ".xclbin";
cl::Program::Binaries bins = xcl::import_binary_file(binary_file);
cl::Program program(context, devices, bins);
cl::Kernel kernel(program,kernel_name_charptr,NULL);

cl::Buffer buffer_input_doc_words;
cl::Buffer buffer_doc_sizes(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, total_num_docs*sizeof(uint),doc_sizes);
cl::Buffer buffer_bloom_filter(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, bloom_filter_size*sizeof(uint),bloom_filter);
cl::Buffer buffer_profile_weights(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, profile_size*sizeof(ulong),profile_weights);
cl::Buffer buffer_fpga_profileScore(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, total_num_docs*sizeof(ulong),fpga_profileScore);

kernel.setArg(2,buffer_bloom_filter);
kernel.setArg(3,buffer_profile_weights);
cl::Buffer sub_buffer;
cl::Buffer doc_sizes_sub_buffer;

chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

 q.enqueueMigrateMemObjects({buffer_profile_weights},0);
 q.enqueueMigrateMemObjects({buffer_bloom_filter},0);

size_per_iter=min(size_per_iter_const,total_doc_size_1);

unsigned int num_iter= (total_doc_size_1/size_per_iter) + ((total_doc_size_1%size_per_iter)>0?1:0);

unsigned int doc_offset=0;

vector<cl::Event> eventlist; 
for(unsigned iter=0;iter<num_iter;iter++) {

unsigned int docs_per_iter = (total_num_docs/num_iter) + ((total_num_docs%num_iter)>iter?1:0);
unsigned int size;
cl::Event ef;
bool flag=false;

if(iter<num_iter-1) {

size = starting_doc_id[doc_offset+docs_per_iter] - starting_doc_id[doc_offset];
} else {
size = total_doc_size_1 - starting_doc_id[doc_offset];
}

cl_buffer_region buffer_info={doc_offset*sizeof(ulong), docs_per_iter*sizeof(ulong)};
cl_buffer_region buffer_info_sizes={doc_offset*sizeof(uint), docs_per_iter*sizeof(uint)};

sub_buffer =  buffer_fpga_profileScore.createSubBuffer(CL_MEM_WRITE_ONLY,CL_BUFFER_CREATE_TYPE_REGION,&buffer_info);
doc_sizes_sub_buffer =  buffer_doc_sizes.createSubBuffer(CL_MEM_READ_ONLY,CL_BUFFER_CREATE_TYPE_REGION,&buffer_info_sizes);

buffer_input_doc_words = cl::Buffer (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, size*sizeof(uint),&input_doc_words[starting_doc_id[doc_offset]]);
if(iter==0) {
kernel.setArg(0,buffer_doc_sizes);
kernel.setArg(4,buffer_fpga_profileScore);
q.enqueueMigrateMemObjects({buffer_doc_sizes},0);
} else {
kernel.setArg(0,doc_sizes_sub_buffer);
kernel.setArg(4,sub_buffer);
}
kernel.setArg(1,buffer_input_doc_words);
kernel.setArg(5,docs_per_iter);
if(iter==0)
 flag=true;
kernel.setArg(6,flag);

q.enqueueMigrateMemObjects({buffer_input_doc_words},0);
q.enqueueTask(kernel,NULL,&ef);
eventlist.push_back(ef);
doc_offset+=docs_per_iter;
}
 
q.enqueueMigrateMemObjects({buffer_fpga_profileScore},CL_MIGRATE_MEM_OBJECT_HOST,&eventlist,NULL);
q.finish();

chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
chrono::duration<double> time_span_fpga  = chrono::duration_cast<duration<double>>(t2-t1);
   
cout << " Execution Time of FPGA is " << time_span_fpga.count() << endl;
}
