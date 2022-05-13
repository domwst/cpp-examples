#pragma once

#include <cstddef>

void RunStress(auto&& f, size_t cnt = -1) {
  while (cnt --> 0) {
    f();
  }
}
