#include "timeMgr.h"

#include <list>
#include <memory>
#include <queue>
#include <utility>
#include <mutex>

#include "logger.h"

namespace XD::Util::TimeMgr
{
  class TimeCallback
  {
  public:
    clock_t         endTime;
    std::function<void()>   cb;
    TimeCallback(std::function<void()> cb, double endTime) :
      endTime((clock_t)endTime), cb(std::move(cb)) {}
  };
  class TimeCallbackCompare
  {
  public:
    bool operator()(const TimeCallback& a, const TimeCallback& b)
    {
      return a.endTime > b.endTime;
    }
  };

  struct _data
  {
  public:
    std::recursive_mutex               mtx;
    std::chrono::high_resolution_clock::time_point startPoint;
    std::priority_queue<
      TimeCallback,
      std::vector<TimeCallback>,
      TimeCallbackCompare> callbacks;
  };
  static std::unique_ptr<_data> _inst = nullptr;

  [[maybe_unused]] void init()
  {
    if (_inst) return;
    _inst = std::make_unique<_data>();
  }

  bool inited() { return (bool)_inst; }

  clock_t now()
  {
    return clock();
  }

  std::chrono::high_resolution_clock::time_point nowTimePoint()
  {
    return std::chrono::high_resolution_clock::now();
  }

  void delay(const std::function<void()>& cb, clock_t delay)
  {
    if (!_inst) return;
    std::lock_guard<std::recursive_mutex> lock(_inst->mtx);
    _inst->callbacks.emplace(cb, delay + now());
  }

  void update()
  {
    if (!_inst) return;
    std::lock_guard<std::recursive_mutex> lock(_inst->mtx);
    if (_inst->callbacks.empty()) return;
    static bool run;
    do
    {
      run = now() >= _inst->callbacks.top().endTime;
      if (run)
      {
        _inst->callbacks.top().cb();
        _inst->callbacks.pop();
      }
    } while (run && !_inst->callbacks.empty());
  }

  void destroy()
  {
    if (!_inst) return;
    _inst.release();
  }
} // namespace XD::Util::TimeMgr
