#pragma once

#include <iostream>

struct C {
  static size_t counter;
  const size_t idx;

  C() : idx(counter++) {
    std::cout << "C() -> C(" << idx << ")" << std::endl;
  }

  C(const C& other) : idx(counter++) {
    std::cout << "C(const C(" << other.idx << ")&) -> C(" << idx << ")" << std::endl;
  }

  C(C&& other) : idx(counter++) {
    std::cout << "C(C(" << other.idx << ")&&) -> C(" << idx << ")" << std::endl;
  }

  C& operator=(const C& other) {
    std::cout << "C(" << idx << ") = C(" << other.idx << ")" << std::endl;
    return *this;
  }

  C& operator=(C&& other) {
    std::cout << "C(" << idx << ") = move(C(" << other.idx << "))" << std::endl;
    return *this;
  }

  ~C() {
    std::cout << "~C(" << idx << ")" << std::endl;
  }
};

size_t C::counter = 0;
