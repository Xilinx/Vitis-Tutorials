/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include <sstream>
#include <fstream> 
#include <iostream> 

#define LERROR   Logger::Log::LogLevel::ERROR
#define LWARN    Logger::Log::LogLevel::WARN
#define LINFO    Logger::Log::LogLevel::INFO
#define LMESSAGE Logger::Log::LogLevel::MESSAGE
#define LNONE Logger::Log::LogLevel::NONE

namespace Logger {

  class Log {
    
  public:

    enum class LogLevel {
      NONE=1, ERROR=1, WARN=2, INFO=3, MESSAGE=4
	};
    
  private:
    LogLevel m_logLevel;
    LogLevel m_coutLevel;
    std::ofstream ofs; 

  public:
    Log();
    ~Log(); 
    
    std::string loglevel2str(LogLevel userLogLevel);     
    void setLogLevel(LogLevel userLogLevel);
    void setCoutLevel(LogLevel userLogLevel);
    void writeLog(LogLevel userLogLevel, const std::string& msg);
   
  };
  
  std::string int2str(int a, bool hex);
  std::string float2str(float a);
  std::string int2float2str(int a);
  void set_level(Log::LogLevel userLogLevel, Log::LogLevel userCoutLevel);
  void flog(Log::LogLevel userLogLevel, const std::string& msg);
}
