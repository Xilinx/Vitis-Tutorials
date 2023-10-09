/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "log.hpp"

namespace Logger {
  
  Log obj;

  //API Functions 
  std::string int2str(int a, bool hex) {
    std::stringstream ss;
    if(hex) {
      ss << "0x" << std::hex << a; 
    }
    else {
      ss << a; 
    }
    return (ss.str()); 
  }

  std::string float2str(float a) {
    std::stringstream ss;
    ss << a;
    return (ss.str());
  }

  std::string int2float2str(int a) {
    float a_float = reinterpret_cast<float &>(a);
    return (float2str(a_float));
  }


  void set_level(Log::LogLevel userLogLevel, Log::LogLevel userCoutLevel) {
    obj.setCoutLevel(userCoutLevel);
    obj.setLogLevel(userLogLevel); 
  }
  
  void flog(Log::LogLevel userLogLevel, std::string msg){
    obj.writeLog(userLogLevel, msg);
  }


  // class methods
  Log::Log() { 
    ofs.open("./run.log"); 
  }

  Log::~Log() {
    ofs.close();
  }

  std::string Log::loglevel2str(LogLevel userLogLevel){
    std::string lvl;	  
    switch(userLogLevel) {
    case LERROR: lvl= "[ERROR]: "; break;
    case LWARN: lvl= "[WARN]: "; break;
    case LINFO: lvl= "[INFO]: "; break;
    case LMESSAGE: lvl= "[MESSAGE]: "; break;	
    default: lvl= "[NONE]: "; break;	
    }
    return lvl;
  }

  void Log::setLogLevel(LogLevel userLogLevel){
    m_logLevel = userLogLevel;
    std::string msg = "Log Level set to " + loglevel2str(userLogLevel);
    Log::writeLog(LogLevel::INFO, msg);
  }

  void Log::setCoutLevel(LogLevel userLogLevel){
    m_coutLevel = userLogLevel;
    std::string msg = "Cout Level set to " + loglevel2str(userLogLevel);
    Log::writeLog(LogLevel::INFO, msg);
  }

  void Log::writeLog(LogLevel userLogLevel, std::string msg) {
    if(userLogLevel <= m_logLevel) {
      std::string lvl;
      
      lvl = loglevel2str(userLogLevel);
      ofs<< lvl << msg << std::endl;
    }

    if(userLogLevel <= m_coutLevel){
      std::string lvl;
      lvl = loglevel2str(userLogLevel);	    
      std::cout<< lvl << msg << std::endl; 
    }
  }
}
