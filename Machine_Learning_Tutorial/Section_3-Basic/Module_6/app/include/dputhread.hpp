#ifndef DPUTHREAD

#define DPUTHREAD

#include <memory>
#include <string>
#include <vitis/ai/bounded_queue.hpp>

#include "decodethread.hpp"
#include "filter.hpp"
#include "frameinfo.hpp"
#include "mythread.hpp"

namespace vitis {
namespace ai {

using queue_t = vitis::ai::BoundedQueue<FrameInfo>;
struct DpuThread : public MyThread {
  DpuThread(std::unique_ptr<Filter>&& filter, queue_t* queue_in,
            queue_t* queue_out,const std::string& suffix);

  virtual ~DpuThread() {}

  virtual int run() override;

  virtual std::string name() override;
  std::unique_ptr<Filter> filter_;
  queue_t* queue_in_;
  queue_t* queue_out_;
  queue_t* queue_show_;
  std::string suffix_;
};
}  // namespace ai
}  // namespace vitis

#endif
