/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "main_xrt.hpp" 

int main(int argc, char ** argv)
{
  std::string msg = ""; 


  //parse commandline args 
  bool profile = OFF;   
  bool print_enable = ON;     
  int num_iter = 1; 
  
  for(int i=0; i<argc; i++) {
    std::cout <<argv[i] <<std::endl;
    if (0 == strcmp(argv[i], "--profile")){
      profile=ON;
    }
    if (0 == strcmp(argv[i], "--timesteps")){
      num_iter = atoi(argv[i+1]);
    }
    if ( (0 == strcmp(argv[i], "--print_enable")) and (0 == strcmp(argv[i+1], "OFF"))){
      print_enable=OFF;
    }
  }
  
  //set log level
  if(print_enable==OFF) {
    Logger::set_level(LMESSAGE,LNONE);
  }
  else{
    Logger::set_level(LMESSAGE,LINFO);
  }
  
  //open xclbin
  Logger::flog(LMESSAGE,"loading a.xclbin ... ");
  auto dhdl = xrtDeviceOpen(0); // Open Device the local device
  if(dhdl == nullptr)
    throw std::runtime_error("No valid device handle found. Make sure using right xclOpen index.");
  auto xclbin = load_xclbin(dhdl, "a.xclbin");
  auto top = reinterpret_cast<const axlf*>(xclbin.data());
  adf::registerXRT(dhdl, top->m_header.uuid);
  Logger::flog(LMESSAGE,"loaded a.xclbin ... ");
  
  int num_i = 32*4;
  int window_size_i = num_i*7;
  int window_size_i_out = 32*7;
  int num_i_particles = num_i*NUM_CU; 

  int num_j_particles = num_i*NUM_CU; 
  int window_size_j = num_j_particles*4;

  int mem_size_i = window_size_i*NUM_CU*sizeof(int);
  int mem_size_i_out = 32*7*NUM_CU*sizeof(int);
  int mem_size_j = window_size_j*sizeof(float);
  
  msg =  "==============================\n"; 
  msg += "Welcome to the NBODY Simulator\n";
  msg += "==============================\n"; 
  msg += "NUM PARTICLES: " + Logger::int2str(num_i_particles,false) + "\n";
  msg += "NUM TIMESTEPS: " + Logger::int2str(num_iter,false) + "\n";
  msg += "==============================\n"; 
  std::cout <<msg;

  if(profile==ON){
    msg = "Profiling turned on ...";
  }
  else {
    msg = "Profiling turned off ... ";
  }
  Logger::flog(LINFO,msg);

  //profile host application nbody 
  NBodySimulator::initalizeNBody(dhdl,num_i_particles,num_j_particles); 
  if(profile==ON){
    auto time_start = std::chrono::steady_clock::now();
    NBodySimulator::simulateNBody(); 
    auto time_stop = std::chrono::steady_clock::now(); 
    std::chrono::duration<double> elapsed_seconds= time_stop - time_start;
    msg = "Elapsed time for NBody Simulator executed in host application is " + Logger::float2str(elapsed_seconds.count()) + " seconds";
    Logger::flog(LINFO,msg);
  }
  
  //read input i data 
  std::vector<int> data_i(window_size_i*NUM_CU); 
  std::string filename = "m2s_i.txt";  
  msg = "reading " + filename + " file ...";
  Logger::flog(LMESSAGE,msg);
  std::ifstream infileI(filename);
  std::string line;
  for(int i=0; i<window_size_i*NUM_CU; i++) {
    std::getline(infileI, line);
    std::istringstream iss(line);
    int val; 
    if (!(iss >> val)) {
      msg = "Error! Cannot cast "+ line+ " to int";
      Logger::flog(LERROR,msg);
      return EXIT_FAILURE; 
    } // error
    data_i[i]=val; 
    msg = "data_i#" + Logger::int2str(i,false) + " int:" + Logger::int2str(val,false) + " float:" + Logger::int2float2str(val); 
    Logger::flog(LMESSAGE,msg);
  }

  //read input j data 
  std::vector<float> data_j(window_size_j); 
  filename = "input_j.txt";  
  msg = "reading " + filename + " file ...";
  Logger::flog(LMESSAGE,msg);
  std::ifstream infileJ(filename);
  for(int i=0; i<window_size_j; i++) {
    std::getline(infileJ, line);
    std::istringstream iss(line);
    float val; 
    if (!(iss >> val)) { 
      msg = "Error! Cannot cast "+ line+ " to float";
      Logger::flog(LERROR,msg);
      return EXIT_FAILURE; 
    } // error
    data_j[i]=val;
    msg = "data_j#" + Logger::int2str(i,false) + " float:" + Logger::float2str(val); 
    Logger::flog(LMESSAGE,msg);
  }

  //read golden output data
  std::vector<long> golden_data_k[4];

  for(int k=0; k<4; k++){
    filename = "s2m_golden_i_k" + Logger::int2str(k,false) + ".txt"; 
    std::ifstream infileIGold(filename);
    msg = "reading " + filename + " file ...";
    Logger::flog(LMESSAGE,msg);
    for (int i = 0; i < window_size_i_out*NUM_CU; i++) {
      std::getline(infileIGold, line);
      std::istringstream iss(line);
      long val; 
      if (!(iss >> val)) { 
	msg = "Error! Cannot cast "+ line+ " to int";
	Logger::flog(LERROR,msg);
	return EXIT_FAILURE;
      } // error
      golden_data_k[k].push_back(val); 
      msg = "kernel_out:" + Logger::int2str(k,false) + "data_i_output_golden#" + Logger::int2str(i,false) + " int:" + Logger::int2str(val,false) + " float:" + Logger::int2float2str(val); 
      Logger::flog(LMESSAGE,msg);
    }
  }
  
  //start iterations 
  int total_error = 0;
  for(int it=0; it<num_iter; it++){
    msg = "Starting interation " + Logger::int2str(it,false) + " ...";
    Logger::flog(LINFO,msg);  
	  
    //allocate output memory
    int *host_out_i_k[4]; 
    xrtBufferHandle out_i_bohdl_k[4];
    
    for(int i=0; i<4; i++){
      out_i_bohdl_k[i] = xrtBOAlloc(dhdl, mem_size_i_out, 0, /*BANK=*/0);
      host_out_i_k[i] = (int*)xrtBOMap(out_i_bohdl_k[i]);
      msg = "allocated and mapped output buffer handle " + Logger::int2str(i,false) + " memory size:" + Logger::int2str(mem_size_i_out,false) + " bytes";
      Logger::flog(LMESSAGE,msg);
    }
    
    //allocate input memory
    xrtBufferHandle in_i_bohdl = xrtBOAlloc(dhdl, mem_size_i, 0, /*BANK=*/0);
    xrtBufferHandle in_j_bohdl = xrtBOAlloc(dhdl, mem_size_j, 0, /*BANK=*/0);
    int *host_in_i = (int*)xrtBOMap(in_i_bohdl);
    float *host_in_j = (float*)xrtBOMap(in_j_bohdl);
    msg = "allocated and mapped input buffer handle i memory size:" + Logger::int2str(mem_size_i,false) + " bytes"; 
    Logger::flog(LMESSAGE,msg);
    msg = "allocated and mapped input buffer handle j memory size:" + Logger::int2str(mem_size_j,false) + " bytes"; 
    Logger::flog(LMESSAGE,msg);
    
    //load i data to ddr
    for(int i=0; i<window_size_i*NUM_CU; i++){
      *(host_in_i+i)=data_i[i];
    }
    
    //load j data to ddr
    for(int i=0; i<window_size_j; i++){
      *(host_in_j+i)=data_j[i];
    }
    
    // sync input memory
    xrtBOSync(in_i_bohdl, XCL_BO_SYNC_BO_TO_DEVICE , mem_size_i,/*OFFSET=*/ 0);
    xrtBOSync(in_j_bohdl, XCL_BO_SYNC_BO_TO_DEVICE , mem_size_j,/*OFFSET=*/ 0);
    Logger::flog(LMESSAGE,"synced input buffer handle i");
    Logger::flog(LMESSAGE,"synced input buffer handle j");
    
    //Open PL kernels handles 
    xrtKernelHandle packet_receiver_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "packet_receiver");	
    Logger::flog(LMESSAGE,"Packet Receiver PL Kernel Opened ... ");
    
    xrtKernelHandle s2m_x4_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "s2m_x4");	
    Logger::flog(LMESSAGE,"S2M_x4 PL Kernel Opened ... ");

    xrtKernelHandle m2s_ij_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "m2s_x2");
    Logger::flog(LMESSAGE,"M2S_x2 PL Kernel Opened ... ");
    
    xrtKernelHandle packet_sender_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "packet_sender");	
    Logger::flog(LMESSAGE,"Packet Sender PL Kernel Opened ... ");
    
    //profile aie nbody 
    auto time_start = std::chrono::steady_clock::now();

    // start output kernels run handles 
    xrtRunHandle packet_receiver_rhdl = xrtKernelRun(packet_receiver_khdl); 
    if(profile==OFF){
      Logger::flog(LMESSAGE,"Packet Receiver PL Kernel run started ... ");
    }
    
    xrtRunHandle s2m_x4_rhdl = xrtKernelRun(s2m_x4_khdl, nullptr, out_i_bohdl_k[0], mem_size_i_out,
					    nullptr, out_i_bohdl_k[1], mem_size_i_out,
					    nullptr, out_i_bohdl_k[2], mem_size_i_out,
					    nullptr, out_i_bohdl_k[3], mem_size_i_out);
    if(profile==OFF){
      Logger::flog(LMESSAGE,"S2M_x4 PL Kernel run started ... ");
    }
    
    // start input kernels run handles
    xrtRunHandle m2s_ij_rhdl = xrtKernelRun(m2s_ij_khdl, 
					    in_i_bohdl, nullptr, mem_size_i, 
					    in_j_bohdl, nullptr, mem_size_j);
    if(profile==OFF){
      Logger::flog(LMESSAGE,"M2S_x2 PL Kernel run started ... ");
    }
     
    xrtRunHandle packet_sender_rhdl = xrtKernelRun(packet_sender_khdl);
    if(profile==OFF){
      Logger::flog(LMESSAGE,"Packet Sender PL Kernel run started ... ");
    }
    
    auto state = xrtRunWait(m2s_ij_rhdl);
    if(profile==OFF){
      msg = "M2S Kernel Run Completed with Status(" + Logger::int2str(state,false) + ")"; 
      Logger::flog(LMESSAGE,msg);
    }

    state = xrtRunWait(packet_sender_rhdl);
    
    if(profile==OFF){
      msg = "Packet Sender Kernel Run Completed with Status(" + Logger::int2str(state,false) + ")"; 
      Logger::flog(LMESSAGE,msg);
    }
       
    // wait for s2m to complete
    state = xrtRunWait(packet_receiver_rhdl);
    if(profile==OFF){
      msg = "Packet Receiver Kernel Run Completed with Status(" + Logger::int2str(state,false) + ")"; 
      Logger::flog(LMESSAGE,msg);
    }

    state = xrtRunWait(s2m_x4_rhdl); 
    if(profile==OFF){
      msg = "S2M Kernel Run Completed with Status(" + Logger::int2str(state,false) + ")"; 
      Logger::flog(LMESSAGE,msg); 
    }

    if(profile==ON){
      auto time_stop = std::chrono::steady_clock::now(); 
      std::chrono::duration<double> elapsed_seconds= time_stop - time_start;
      msg = "Elapsed time for NBody Simulator executed in AI Engine is " + Logger::float2str(elapsed_seconds.count()) + " seconds";
      Logger::flog(LINFO,msg);
    }

    // sync output memory
    for(int i=0; i<4; i++){
      xrtBOSync(out_i_bohdl_k[i], XCL_BO_SYNC_BO_FROM_DEVICE , mem_size_i_out,/*OFFSET=*/ 0);
      msg ="synced output buffer handle " +  Logger::int2str(i,false); 
      Logger::flog(LMESSAGE,msg);
    }
    
    int diff; 
      
    // post-processing data
    if(profile==ON) {
      msg = "Profiling enabled, skipping data check ... ";
      Logger::flog(LMESSAGE,msg);
    }
    else { //profiling is turned off, and we want to do a data check 
      int packet_num =0;   
      for (int cu =0; cu < NUM_CU; cu++) {
	for (int k = 0; k<4; k++){
	  for (int i = 0; i < window_size_i_out; i++) {  
	    diff =  golden_data_k[k][(cu*window_size_i_out)+i] ^ *(host_out_i_k[k]+(cu*window_size_i_out)+i);
	    if (diff!=0) {
	      total_error += 1;
	    } 
	    msg = " packet#" + Logger::int2str(packet_num, false);
	    msg +=" compute_unit#:" + Logger::int2str(cu,false); 
	    msg +=" kernel:" + Logger::int2str(k,false); 
	    msg +=" Data i:" + Logger::int2str(i,false);
	    int val = host_in_i[i+(cu*window_size_i)]; 
	    msg +=" in_i:" + Logger::int2float2str(val); 
	    val = *(host_out_i_k[k]+ (cu*window_size_i_out) + i); 
	    msg +=" out_i:" + Logger::int2float2str(val);
	    val = golden_data_k[k][(cu*window_size_i_out)+i]; 
	    msg +=" out_i_expected:" + Logger::int2float2str(val);
	    msg +=" diff:" + Logger::int2str(diff,false); 
	    msg +=" err:" + Logger::int2str(total_error,false); 
	    
	    if(diff !=0){
	      Logger::flog(LERROR,msg);
	    }
	    else {
	      Logger::flog(LMESSAGE,msg);
	    }
	   
	    packet_num+=1;
	  }
	}
      }
    }
    

    // release memory
    xrtRunClose(packet_receiver_rhdl);
    xrtKernelClose(packet_receiver_khdl);
    Logger::flog(LMESSAGE,"Packet Receiver Kernel/Run Handles closed ...");
    
    xrtRunClose(s2m_x4_rhdl);
    xrtKernelClose(s2m_x4_khdl);
    Logger::flog(LMESSAGE,"S2M Kernel/Run Handles closed ...");
    
    xrtRunClose(m2s_ij_rhdl);
    xrtKernelClose(m2s_ij_khdl);
    Logger::flog(LMESSAGE,"M2S Kernel/Run Handles closed ...");
    
    xrtRunClose(packet_sender_rhdl);
    xrtKernelClose(packet_sender_khdl);
    Logger::flog(LMESSAGE,"Packet Sender Kernel/Run Handles closed ...");
    
    for(int i=0; i<4; i++){
      xrtBOFree(out_i_bohdl_k[i]);
      msg = "Output Buffer Handler " + Logger::int2str(i,false) + "freed ...";
      Logger::flog(LMESSAGE,msg);
    }
    
    xrtBOFree(in_i_bohdl);
    xrtBOFree(in_j_bohdl);
    Logger::flog(LMESSAGE,"Input Buffer Handle i freed ...");
    Logger::flog(LMESSAGE,"Input Buffer Handle j freed ...");
    
  }
    
  xrtDeviceClose(dhdl);
  Logger::flog(LMESSAGE,"Closed a.xclbin ...");
  
  std::string test_result = total_error ? "FAILED" : "PASSED"; 
  msg = "TEST " + test_result; 
  Logger::flog(LINFO,msg);    
  return (total_error ? EXIT_FAILURE :  EXIT_SUCCESS);	
  
}
