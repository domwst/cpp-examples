#pragma once

#include <cstdint>
#include <ctime>

class StopWatch {
 public:
  StopWatch() : start_(clock()) {
  }

  [[nodiscard]] uint64_t ElapsedMillis() const {
    return (clock() - start_) * 1000 / CLOCKS_PER_SEC;
  }

 public:
  uint64_t start_;
};
