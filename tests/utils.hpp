#pragma once

#include <random>
#include <string>
#include <limits>

std::vector<std::string> random_strings(size_t n, size_t len = 30, size_t seed = 0)
{
  if (seed == 0)
    seed = std::random_device()();
  
  std::vector<std::string> xs;
  xs.reserve(n);
  std::default_random_engine e(seed);
  std::uniform_int_distribution<int> u(97,122); // a-z ascii codes

  for (int i = 0; i < n; ++i)
  {
    std::string x;
    x.reserve(len);
    for (int j = 0; j < len; ++j)
      x.push_back((char)u(e));
    xs.push_back(x);
  }
  return xs;
}

template <typename T>
std::vector<T> random_elements(
  size_t n,
  T low = std::numeric_limits<T>::min(),
  T high = std::numeric_limits<T>::max(),
  size_t seed = 0)
{
  if (seed == 0)
    seed = std::random_device()();
  
  std::vector<T> xs;
  xs.reserve(n);
  std::default_random_engine e(seed);
  std::uniform_int_distribution<T> u(low,high);

  for (size_t i = 0; i < n; ++i)
    xs.push_back(u(e));

  return xs;
}