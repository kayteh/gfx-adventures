#pragma once
#include <map>
#include <chrono>
#include <functional>

namespace deferrable {

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

static std::map<TimePoint, std::function<void()>> _deferrables;

class defer {
public:
  defer(const defer &) = delete;

  // template <c
  inline defer(std::function<void()> fn,
               std::chrono::duration<long long, std::milli> waitDuration) {
    auto now = Clock::now();
    TimePoint timeout = now + waitDuration;
    _deferrables.insert_or_assign(timeout, fn);
  }
};

static void tick() {
  auto now = Clock::now();
  std::map<TimePoint, std::function<void()>> defCopy(_deferrables);
  for (auto const &d : defCopy) {
    if (d.first < now) {
      d.second();
      _deferrables.erase(d.first);
    }
  }
}

} // namespace deferrable