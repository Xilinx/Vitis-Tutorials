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
unsigned int num_compute_units=2;

cl::Context context(device);
cl::CommandQueue q(context,device,CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);

string run_type = xcl::is_emulation()?(xcl::is_hw_emulation()?"hw_emu":"sw_emu"):"hw";
string binary_file = kernel_name + "_" + run_type + ".xclbin";
cl::Program::Binaries bins = xcl::import_binary_file(binary_file);
cl::Program program(context, devices, bins);
vector<cl::Kernel> kernel(num_compute_units);

for(unsigned int i=0;i<num_compute_units;i++) {
string s = kernel_name+":{"+kernel_name + "_"+ to_string(i+1)+"}";

kernel[i] = cl::Kernel(program,s.c_str(),NULL);
}


cl::Buffer buffer_input_doc_words[num_compute_units];
cl::Buffer buffer_doc_sizes(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,total_num_docs*sizeof(uint),doc_sizes);
cl::Buffer buffer_bloom_filter[num_compute_units];
cl::Buffer buffer_profile_weights[num_compute_units];
cl::Buffer buffer_fpga_profileScore(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, total_num_docs*sizeof(ulong),fpga_profileScore);
cl::Buffer sub_buffer[num_compute_units];
cl::Buffer doc_sizes_sub_buffer[num_compute_units];

for(unsigned int i=0;i<num_compute_units;i++) {
buffer_bloom_filter[i] =  cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, bloom_filter_size*sizeof(uint),bloom_filter);
buffer_profile_weights[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, profile_size*sizeof(ulong),profile_weights);
kernel[i].setArg(2,buffer_bloom_filter[i]);
kernel[i].setArg(3,buffer_profile_weights[i]);
}

chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

for(unsigned int i=0;i<num_compute_units;i++) {
 q.enqueueMigrateMemObjects({buffer_profile_weights[i]},0);
 q.enqueueMigrateMemObjects({buffer_bloom_filter[i]},0);
}


unsigned int doc_offset[num_compute_units]={0};
 vector<cl::Event> eventlist;
 unsigned int num_docs[num_compute_units];
 unsigned int total_size[num_compute_units]; 
 unsigned int num_iter=0;
unsigned int doc_offset_per_iter[num_compute_units]; 

for(unsigned int comp=0;comp<num_compute_units;comp++) {
  
  doc_offset_per_iter[comp]=doc_offset[comp];
  num_docs[comp] = (total_num_docs/num_compute_units) + (((total_num_docs%num_compute_units)>comp)?1:0);
  
  if(comp<num_compute_units-1) {
   total_size[comp] = starting_doc_id[(num_docs[comp]+doc_offset[comp])]-starting_doc_id[doc_offset[comp]];
  doc_offset[comp+1]=(doc_offset[comp])+num_docs[comp];
  }
  else
   total_size[comp] =  (total_doc_size_1-starting_doc_id[doc_offset[comp]]);
  
  
  size_per_iter=min(total_size[comp],size_per_iter_const);
  
  num_iter = max(num_iter,((total_size[comp]/size_per_iter) + (((total_size[comp]%size_per_iter)>0)?1:0)));
  }


for(unsigned int i=0;i<num_iter;i++) {

for(unsigned int comp=0;comp<num_compute_units;comp++) {

cl::Event eve,ef,eve_size;
vector<cl::Event> waitlist;
 unsigned int docs_per_iter = (num_docs[comp]/num_iter) + (((num_docs[comp]%num_iter)>i)?1:0);
unsigned int size;
bool flag=false;

if(i<num_iter-1)
size= (starting_doc_id[docs_per_iter+doc_offset_per_iter[comp]]) - (starting_doc_id[doc_offset_per_iter[comp]]);
else
size= starting_doc_id[doc_offset[comp]] + total_size[comp] - (starting_doc_id[doc_offset_per_iter[comp]]);

buffer_input_doc_words[comp]=cl::Buffer(context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,size*sizeof(uint),input_doc_words+starting_doc_id[doc_offset_per_iter[comp]]);
cl_buffer_region buffer_info={doc_offset_per_iter[comp]*sizeof(ulong), docs_per_iter*sizeof(ulong)};
cl_buffer_region buffer_info_sizes={doc_offset_per_iter[comp]*sizeof(uint), docs_per_iter*sizeof(uint)};

sub_buffer[comp] =  buffer_fpga_profileScore.createSubBuffer(CL_MEM_WRITE_ONLY,CL_BUFFER_CREATE_TYPE_REGION,&buffer_info);
doc_sizes_sub_buffer[comp] =  buffer_doc_sizes.createSubBuffer(CL_MEM_READ_ONLY,CL_BUFFER_CREATE_TYPE_REGION,&buffer_info_sizes);

//kernel[comp].setArg(0,doc_sizes_sub_buffer[comp]);
if(i==0 && comp==0){ 
kernel[comp].setArg(0,buffer_doc_sizes);
kernel[comp].setArg(4,buffer_fpga_profileScore);
q.enqueueMigrateMemObjects({buffer_doc_sizes},0);
}
else {
kernel[comp].setArg(0,doc_sizes_sub_buffer[comp]);
kernel[comp].setArg(4,sub_buffer[comp]);
}
kernel[comp].setArg(1,buffer_input_doc_words[comp]);
kernel[comp].setArg(5,docs_per_iter);
//kernel[comp].setArg(6,doc_offset_per_iter[comp]);
kernel[comp].setArg(6,size);
if(i==0) 
 flag=true;
kernel[comp].setArg(7,flag);

q.enqueueMigrateMemObjects({buffer_input_doc_words[comp]},0,NULL,&eve);
waitlist.push_back(eve);
q.enqueueTask(kernel[comp],&waitlist,&ef);
eventlist.push_back(ef);
doc_offset_per_iter[comp]+=docs_per_iter;
}
}
q.enqueueMigrateMemObjects({buffer_fpga_profileScore},CL_MIGRATE_MEM_OBJECT_HOST,&eventlist,NULL);
q.finish();

   chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
   chrono::duration<double> time_span_fpga  = chrono::duration_cast<duration<double>>(t2-t1);
cout << "Execution time of FPGA is " << time_span_fpga.count() << endl;   

}
