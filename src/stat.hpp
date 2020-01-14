#pragma once

#include <ostream>
#include <stdlib.h>

// since static fields are shared across multiple instances of a class
// I use int parameter to distinguish between different examples in single compilation unit
template <int N>
struct cnt {
  static uint32_t constructed;
  static uint32_t destroyed;
  static uint32_t copied;
  static uint32_t moved;
  static uint32_t copy_assigned;
  static uint32_t move_assigned;
  size_t value{0};
  
  cnt() {
    ++constructed;
  }

  cnt(size_t v): value(v) {
      ++constructed;
  }
  
  virtual ~cnt() {
    ++destroyed;
  };
  
  cnt(const cnt& rhs): value(rhs.value) {
    ++copied;
  }
  
  cnt(cnt&& rhs) noexcept : value(rhs.value) {
    ++moved;
  }
  
  cnt& operator = (const cnt& rhs) {
    value = rhs.value;
    ++copy_assigned;
    return *this;
  }
  
  cnt& operator = (cnt&& rhs) noexcept {
    value = rhs.value;
    ++move_assigned;
    return *this;
  }
};

template <int N>
uint32_t cnt<N>::constructed = 0;
template <int N>
uint32_t cnt<N>::destroyed = 0;
template <int N>
uint32_t cnt<N>::copied = 0;
template <int N>
uint32_t cnt<N>::moved = 0;
template <int N>
uint32_t cnt<N>::copy_assigned = 0;
template <int N>
uint32_t cnt<N>::move_assigned = 0;

template <int N>
std::ostream& operator << (std::ostream& s, const cnt<N>&) {
  s << "cons: " << cnt<N>::constructed
    << ", dstr: " << cnt<N>::destroyed
    << ", cp: " << cnt<N>::copied
    << ", mv: " << cnt<N>::moved
    << ", cpa: " << cnt<N>::copy_assigned
    << ", mva: " << cnt<N>::move_assigned;

    return s;
}