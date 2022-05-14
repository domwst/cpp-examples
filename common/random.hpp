#include <random>
#include <chrono>

size_t Rand() {
  static std::mt19937_64 rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  return rnd();
}

size_t Rand(size_t l, size_t r) {
  return Rand() % (r - l) + l;
}
