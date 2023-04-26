/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include <iostream> 
#include <cmath>
#include <sstream>
#include <fstream> 

#include "experimental/xrt_kernel.h"

namespace NBodySimulator {

  class NBody {

  private:     
    int m_numParticlesi;
    int m_numParticlesj; 
    
    xrtBufferHandle m_x_bohdl; 
    xrtBufferHandle m_y_bohdl; 
    xrtBufferHandle m_z_bohdl; 
    xrtBufferHandle m_vx_bohdl; 
    xrtBufferHandle m_vy_bohdl; 
    xrtBufferHandle m_vz_bohdl; 
    xrtBufferHandle m_m_bohdl; 

    float *m_x_ptr;
    float *m_y_ptr;
    float *m_z_ptr;
    float *m_vx_ptr;
    float *m_vy_ptr;
    float *m_vz_ptr;
    float *m_m_ptr;
    
    int m_mem_size;
    xrtDeviceHandle m_dhdl;

    xrtBufferHandle m_datai_bohdl; 
    int *m_datai_ptr;

    xrtBufferHandle m_dataj_bohdl; 
    float *m_dataj_ptr;

    xrtBufferHandle m_xNew_bohdl; 
    xrtBufferHandle m_yNew_bohdl; 
    xrtBufferHandle m_zNew_bohdl; 
    xrtBufferHandle m_vxNew_bohdl; 
    xrtBufferHandle m_vyNew_bohdl; 
    xrtBufferHandle m_vzNew_bohdl; 
    xrtBufferHandle m_mNew_bohdl; 

    float *m_xNew_ptr;
    float *m_yNew_ptr;
    float *m_zNew_ptr;
    float *m_vxNew_ptr;
    float *m_vyNew_ptr;
    float *m_vzNew_ptr;
    float *m_mNew_ptr;

    int seed = 12; 
    float minPos = -1000;  
    float maxPos = 1000;
    float minV = -2.0; 
    float maxV = 2.0; 
    float minMass = 0; 
    float maxMass = 1000; 
    float softeningFactor2 = 1000;
    float timestep = 1; 
    
    int m_packet_size = 224; //32x+32y+32z+32vx+32vy+32vz+32m
    int m_numI = 32;
    int m_numIDataTypes = 7; //x,y,z,vx,vy,vz,m
    int m_numJDataTypes = 4; //x,y,z,m
    
    std::ofstream m_ofs; 

  public:
    NBody();
    ~NBody();

    void initalizeParticles(xrtDeviceHandle dh, int userNumParticlesi,int userNumParticlesj);
    void simulate();
    int * generateDataI();
    float * generateDataJ();
    int getNumPackets();
    void updateDataFromPtr(int * userDataI);
    void saveData();

  };
  
  //API functions
  float generateRandomFloat(float LO, float HI);
  int * getDataI();
  float * getDataJ();
  void simulateNBody();
  void initalizeNBody(xrtDeviceHandle dh, int userNumParticlesi,int userNumParticlesj);
  float int2float(int a);
  int float2int(float a);
  void updateNBodyFromPtr(int * userDataI);
  void saveAnimationData();
  std::vector<float> linspace(float start_in, float end_in, int num_in);

}
