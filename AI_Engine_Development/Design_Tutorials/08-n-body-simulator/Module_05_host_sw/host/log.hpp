/* (c) Copyright 2021 Xilinx, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 * 		http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
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
      ERROR=1, WARN=2, INFO=3, MESSAGE=4, NONE=0
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

  //API Functions 
  std::string int2str(int a, bool hex);
  std::string float2str(float a);
  std::string int2float2str(int a);
  void set_level(Log::LogLevel userLogLevel, Log::LogLevel userCoutLevel);
  void flog(Log::LogLevel userLogLevel, const std::string& msg);
}
