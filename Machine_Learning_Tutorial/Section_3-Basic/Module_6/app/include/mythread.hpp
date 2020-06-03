#pragma once

#include <glog/logging.h>
#include <signal.h>
#include <memory>
#include <vector>
#include <thread>
#include "vitis/ai/env_config.hpp"



#ifndef USE_DRM
#define USE_DRM 0
#endif

#if USE_DRM
#include <vitis/ai/dpdrm.hpp>
#endif

namespace vitis {
namespace ai {
DEF_ENV_PARAM(DEBUG_DEMO, "1")
struct MyThread {
  // static std::vector<MyThread *> all_threads_;
  static inline std::vector<MyThread*>& all_threads();
  static void signal_handler(int);
  static void stop_all();
  static void wait_all();
  static void start_all();

  static void main_proxy(MyThread* me);
  void main();

  virtual int run() = 0;

  virtual std::string name() = 0;

  explicit MyThread();
  virtual ~MyThread();

  void start();

  void stop();
  void wait();
  bool is_stopped();

  bool stop_;
  std::unique_ptr<std::thread> thread_;
};
}//end ai
}//end vitis
