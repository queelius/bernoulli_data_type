#include <bernoulli/bernoulli_set_builder.hpp>
#include <hashing/fnv_hash.hpp>
#include <bernoulli/bernoulli_set.hpp>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <chrono>
#include "utils.hpp"

void test_bernoulli_set();

int main()
{
  test_bernoulli_set();
}

void test_bernoulli_set()
{
  auto xs = random_strings(200);
  auto start = std::chrono::system_clock::now();
  auto bs = bernoulli::bernoulli_set_builder<hashing::fnv_hash>().
    false_positive_rate(0.01).
    timeout(std::chrono::seconds(100))(xs);

  auto end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "elapsed = " << (double)elapsed.count() / 1000 << '\n';
  std::cout << "fpr = " << bs.false_positive_rate() * 100 << "%\n";
  std::cout << "fnr = " << bs.false_negative_rate() * 100 << "%\n";

  size_t test_fn = 0;
  for (auto x : xs)
  {
      auto res = bs.contains(x);
      if (!res)
          ++test_fn;
  }
  std::cout << "sampled fnr = " << (double)test_fn / xs.size() << "\n";

  size_t fp = 0;
  size_t n = 100000;
  auto ys = random_strings(n);
  for (auto y : ys)
  {
    if (bs.contains(y))
      ++fp;
  }

  std::cout << "sampled fpr = " << (double)fp / n << "\n";
}

