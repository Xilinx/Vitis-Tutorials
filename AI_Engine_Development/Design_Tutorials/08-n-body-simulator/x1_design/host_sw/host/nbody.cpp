/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "nbody.hpp"
#include "log.hpp"

namespace NBodySimulator {
  NBody obj; 
  
  //class methods
  NBody::NBody() {
    m_ofs.open("./animation_data.txt"); 
  } 
 
  NBody::~NBody() {
    m_ofs.close();

    xrtBOFree(m_x_bohdl);
    xrtBOFree(m_y_bohdl); 
    xrtBOFree(m_z_bohdl); 
    xrtBOFree(m_vx_bohdl); 
    xrtBOFree(m_vy_bohdl); 
    xrtBOFree(m_vz_bohdl); 
    xrtBOFree(m_m_bohdl); 

    xrtBOFree(m_datai_bohdl); 
    xrtBOFree(m_dataj_bohdl); 

    xrtBOFree(m_xNew_bohdl);
    xrtBOFree(m_yNew_bohdl); 
    xrtBOFree(m_zNew_bohdl); 
    xrtBOFree(m_vxNew_bohdl); 
    xrtBOFree(m_vyNew_bohdl); 
    xrtBOFree(m_vzNew_bohdl); 
    xrtBOFree(m_mNew_bohdl); 
  }
  
  
  void NBody::saveData(){
    for(int i=0; i<m_numParticlesi; i++){
      std::string msg = Logger::float2str(m_x_ptr[i]) + " "; 
      msg += Logger::float2str(m_y_ptr[i]) + " ";
      msg += Logger::float2str(m_z_ptr[i]) + " ";
      msg += Logger::float2str(m_m_ptr[i]);
      m_ofs << msg << std::endl;
    }
  }
  
  void NBody::initalizeParticles(xrtDeviceHandle dh, int userNumParticlesi,int userNumParticlesj) {
    m_numParticlesi = userNumParticlesi; 
    m_numParticlesj = userNumParticlesj; 
    m_mem_size = m_numParticlesi*sizeof(float); 
    m_dhdl = dh; 

    m_x_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);
    m_y_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);
    m_z_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);
    m_vx_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);
    m_vy_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);
    m_vz_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);
    m_m_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);

    m_x_ptr=(float*)xrtBOMap(m_x_bohdl);
    m_y_ptr=(float*)xrtBOMap(m_y_bohdl);
    m_z_ptr=(float*)xrtBOMap(m_z_bohdl);
    m_vx_ptr=(float*)xrtBOMap(m_vx_bohdl);
    m_vy_ptr=(float*)xrtBOMap(m_vy_bohdl);
    m_vz_ptr=(float*)xrtBOMap(m_vz_bohdl);
    m_m_ptr=(float*)xrtBOMap(m_m_bohdl);

    m_xNew_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);
    m_yNew_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);
    m_zNew_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);
    m_vxNew_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);
    m_vyNew_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);
    m_vzNew_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);
    m_mNew_bohdl=xrtBOAlloc(m_dhdl, m_mem_size, 0, /*BANK=*/0);

    m_xNew_ptr=(float*)xrtBOMap(m_xNew_bohdl);
    m_yNew_ptr=(float*)xrtBOMap(m_yNew_bohdl);
    m_zNew_ptr=(float*)xrtBOMap(m_zNew_bohdl);
    m_vxNew_ptr=(float*)xrtBOMap(m_vxNew_bohdl);
    m_vyNew_ptr=(float*)xrtBOMap(m_vyNew_bohdl);
    m_vzNew_ptr=(float*)xrtBOMap(m_vzNew_bohdl);
    m_mNew_ptr=(float*)xrtBOMap(m_mNew_bohdl);

    int mem_size=m_numParticlesi*7*sizeof(int); 
    m_datai_bohdl=xrtBOAlloc(m_dhdl, mem_size, 0, /*BANK=*/0);
    m_datai_ptr=(int*)xrtBOMap(m_datai_bohdl);

    mem_size=m_numParticlesj*4*sizeof(float); 
    m_dataj_bohdl=xrtBOAlloc(m_dhdl, mem_size, 0, /*BANK=*/0);
    m_dataj_ptr=(float*)xrtBOMap(m_dataj_bohdl);
    
    srand(seed);

    std::vector<float> theta = linspace(0,2*M_PI,m_numParticlesi);

    float radius = 1000.0; 

    for(int i=0; i<m_numParticlesi; i++) {
      float r = generateRandomFloat(0,1);
      float x=radius*r*cos(theta[i]);
      float y=radius*r*sin(theta[i]);
      m_x_ptr[i] = x;
      m_y_ptr[i] = y;
      m_z_ptr[i] = generateRandomFloat(minPos,maxPos);
      m_vx_ptr[i] = generateRandomFloat(minV,maxV);
      m_vy_ptr[i] = generateRandomFloat(minV,maxV);
      m_vz_ptr[i] = generateRandomFloat(minV,maxV);
      m_m_ptr[i] = generateRandomFloat(10,110);
      m_xNew_ptr[i] = 0;
      m_yNew_ptr[i] = 0;
      m_zNew_ptr[i] = 0;
      m_vxNew_ptr[i] = 0;
      m_vyNew_ptr[i] = 0;
      m_vzNew_ptr[i] = 0;
      m_mNew_ptr[i] = m_m_ptr[i]; 
    }

    /*
    for(int i=0; i<m_numParticlesi; i++) {
      m_x_ptr[i] = generateRandomFloat(minPos,maxPos);
      m_y_ptr[i] = generateRandomFloat(minPos,maxPos);
      m_z_ptr[i] = generateRandomFloat(minPos,maxPos);
      m_vx_ptr[i] = generateRandomFloat(minV,maxV);
      m_vy_ptr[i] = generateRandomFloat(minV,maxV);
      m_vz_ptr[i] = generateRandomFloat(minV,maxV);
      m_m_ptr[i] = generateRandomFloat(minMass,maxMass);
      
      m_xNew_ptr[i] = 0;
      m_yNew_ptr[i] = 0;
      m_zNew_ptr[i] = 0;
      m_vxNew_ptr[i] = 0;
      m_vyNew_ptr[i] = 0;
      m_vzNew_ptr[i] = 0;
      m_mNew_ptr[i] = m_m_ptr[i]; 
    } 
    */   
  }

  int NBody::getNumPackets() {
    int num_packets = m_numParticlesi/m_numI;
    return num_packets;
  }

  float * NBody::generateDataJ() {
    int num_packets = getNumPackets();
    
    int idx =0;
    for(int p=0; p<num_packets; p++){
      for(int i=0; i<m_numI; i++) {
	float data = m_x_ptr[i+(p*m_numI)];
	m_dataj_ptr[idx]=data;
	idx+=1;
      }
      for(int i=0; i<m_numI; i++) {
	float data = m_y_ptr[i+(p*m_numI)];
	m_dataj_ptr[idx]=data;
	idx+=1;
      }
      for(int i=0; i<m_numI; i++) {
	float data = m_z_ptr[i+(p*m_numI)];
	m_dataj_ptr[idx]=data;
	idx+=1;
      }
      for(int i=0; i<m_numI; i++) {
	float data = m_m_ptr[i+(p*m_numI)];
	m_dataj_ptr[idx]=data;
	idx+=1;
      }
    } 
    
    return m_dataj_ptr;
  }

  int * NBody::generateDataI() {
    int num_packets = getNumPackets();
    int idx =0;
    
    for(int p=0; p<num_packets; p++){
      for(int i=0; i<m_numI; i++) {
	float data = m_x_ptr[i+(p*m_numI)];
	int data_int = float2int(data); 
	m_datai_ptr[idx]=data_int;
	idx+=1;
      }
      for(int i=0; i<m_numI; i++) {
	float data = m_y_ptr[i+(p*m_numI)];
	int data_int = float2int(data); 
	m_datai_ptr[idx]=data_int;
	idx+=1;
      }
      for(int i=0; i<m_numI; i++) {
	float data = m_z_ptr[i+(p*m_numI)];
	int data_int = float2int(data); 
	m_datai_ptr[idx]=data_int;
	idx+=1;
      }
      for(int i=0; i<m_numI; i++) {
	float data = m_vx_ptr[i+(p*m_numI)];
	int data_int = float2int(data); 
	m_datai_ptr[idx]=data_int;
	idx+=1;
      }
      for(int i=0; i<m_numI; i++) {
	float data = m_vy_ptr[i+(p*m_numI)];
	int data_int = float2int(data); 
	m_datai_ptr[idx]=data_int;
	idx+=1;
      }
      for(int i=0; i<m_numI; i++) {
	float data = m_vz_ptr[i+(p*m_numI)];
	int data_int = float2int(data); 
	m_datai_ptr[idx]=data_int;
	idx+=1;
      }
      for(int i=0; i<m_numI; i++) {
	float data = m_m_ptr[i+(p*m_numI)];
	int data_int = float2int(data); 
	m_datai_ptr[idx]=data_int;
	idx+=1;
      }
    }
    
    return m_datai_ptr;
  }

  void NBody::updateDataFromPtr(int * userDataI) {
    int num_packets = getNumPackets();
    int idx_x=0;
    int idx_y=0;
    int idx_z=0;
    int idx_vx=0;
    int idx_vy=0;
    int idx_vz=0;
    int idx_m=0;
    
    for(int p=0; p<num_packets; p++) {
      for(int i=0; i<m_packet_size; i++){
	if(i>=0 and i<32) {
	  int data=userDataI[i+p*m_packet_size]; 
	  float data_float = int2float(data);
	  m_x_ptr[idx_x]=data_float;
	  idx_x +=1;
	}
	else if(i>=32 and i<64){
	  m_y_ptr[idx_y]=int2float(userDataI[i+p*m_packet_size]);
	  idx_y +=1;
	}
	else if(i>=64 and i<96) {
	  m_z_ptr[idx_z]=int2float(userDataI[i+p*m_packet_size]);
	  idx_z +=1;
	}
	else if(i>=96 and i<128) {
	  m_vx_ptr[idx_vx]=int2float(userDataI[i+p*m_packet_size]);
	  idx_vx +=1;
	}
	else if(i>=128 and i<160) {
	  m_vy_ptr[idx_vy]=int2float(userDataI[i+p*m_packet_size]);
	  idx_vy +=1;
	}
	else if(i>=160 and i<192) {
	  m_vz_ptr[idx_vz]=int2float(userDataI[i+p*m_packet_size]);
	  idx_vz +=1;
	}
	else if(i>=192 and i<224) {
	  m_m_ptr[idx_m]=int2float(userDataI[i+p*m_packet_size]);
	  idx_m +=1;
	}	
      }
    }
  }
  
  void NBody::simulate() {   
    int mem_size = m_mem_size; 
    xrtDeviceHandle dhdl =m_dhdl;
    
    xrtBufferHandle accum_accx_bohdl=xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
    xrtBufferHandle accum_accy_bohdl=xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
    xrtBufferHandle accum_accz_bohdl=xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
    
    float *accum_accx_ptr=(float*)xrtBOMap(accum_accx_bohdl);
    float *accum_accy_ptr=(float*)xrtBOMap(accum_accy_bohdl);
    float *accum_accz_ptr=(float*)xrtBOMap(accum_accz_bohdl);
    

    for(int i=0; i<m_numParticlesi; i++) {
      float xi = m_x_ptr[i]; 
      float yi = m_y_ptr[i];
      float zi = m_z_ptr[i];
      
      accum_accx_ptr[i] = 0;  
      accum_accy_ptr[i] = 0;  
      accum_accz_ptr[i] = 0;  
      
      for(int j=0; j<m_numParticlesj;j++){
	float xj = m_x_ptr[j];
	float yj = m_y_ptr[j];
	float zj = m_z_ptr[j];
	
	float dx = xj-xi;
	float dy = yj-yi;
	float dz = zj-zi;
	
	//l2dist = dx^2 dy^2 + dz^2 + sf2
	float l2dist = pow(dx,2) + pow(dy,2) + pow(dz,2) + softeningFactor2; 
	
	//r = invsqrt(l2dist***3) 
	float r = pow(l2dist,3);
	r = 1/sqrt(r);
	
	//s = r*m
	float s = r*m_m_ptr[j]; 
	
	//acceleration
	accum_accx_ptr[i] += dx*s; 
	accum_accy_ptr[i] += dy*s; 
	accum_accz_ptr[i] += dz*s; 
      }
    }
    
    //calculate new positions     
    for(int i=0; i<m_numParticlesi; i++) {
      m_xNew_ptr[i]=m_x_ptr[i]*m_vx_ptr[i];
      m_yNew_ptr[i]=m_y_ptr[i]*m_vy_ptr[i];
      m_zNew_ptr[i]=m_z_ptr[i]*m_vz_ptr[i];
    }
    
    //calculate new velocities
    for(int i=0; i<m_numParticlesi; i++) {
      m_vxNew_ptr[i]=m_vx_ptr[i]+(accum_accx_ptr[i]*timestep);
      m_vyNew_ptr[i]=m_vy_ptr[i]+(accum_accy_ptr[i]*timestep);
      m_vzNew_ptr[i]=m_vz_ptr[i]+(accum_accz_ptr[i]*timestep);
    }
    
    xrtBOFree(accum_accx_bohdl);
    xrtBOFree(accum_accy_bohdl); 
    xrtBOFree(accum_accz_bohdl); 
  }
  
  //API functions
  float generateRandomFloat(float LO, float HI) {
    float result = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    return result;
  }
  
  void saveAnimationData() {
    obj.saveData();
  }

  int float2int(float a) {
    int a_int = reinterpret_cast<int &>(a);
    return a_int;    
  }

  float int2float(int a) {
    float a_float = reinterpret_cast<float &>(a);
    return a_float;
  }

  int * getDataI() {
    return obj.generateDataI();
  }

  float * getDataJ() {
    return obj.generateDataJ();
  }

  
  void simulateNBody() {    
    obj.simulate(); 
  }
  
  void initalizeNBody(xrtDeviceHandle dh, int userNumParticlesi, int userNumParticlesj) {    
    obj.initalizeParticles(dh,userNumParticlesi,userNumParticlesj);
  }

  void updateNBodyFromPtr(int * userDataI){
    obj.updateDataFromPtr(userDataI);
  }


  std::vector<float> linspace(float start_in, float end_in, int num_in)
  {

    std::vector<float> linspaced;

    float start = static_cast<float>(start_in);
    float end = static_cast<float>(end_in);
    float num = static_cast<float>(num_in);

    if (num == 0) { return linspaced; }

    if (num == 1) 
      {
	linspaced.push_back(start);
	return linspaced;
      }

    float delta = (end - start) / (num - 1);

    for(int i=0; i < num-1; ++i)
      {
	linspaced.push_back(start + delta * i);
      }

    linspaced.push_back(end);
    return linspaced;
  }
  
}
