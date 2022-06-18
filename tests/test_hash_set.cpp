#include <hash_set/hash_set_builder.hpp>
#include <hashing/fnv_hash.hpp>
#include <hash_set/hash_set.hpp>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <chrono>
#include "utils.hpp"

void test_hash_set(size_t n, double fpr=0.1, size_t timeout = 120);

int main()
{
  test_hash_set(50,.1);
}

void test_hash_set(size_t n, double fpr, size_t timeout)
{
  auto xs = random_strings(n);
  auto start = std::chrono::system_clock::now();
  auto bs = bernoulli::hash_set_builder<hashing::fnv_hash>().
    false_positive_rate(fpr).debugging().
    timeout(std::chrono::seconds(timeout))(xs);

  auto end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

  std::cout << "elapsed: " << (double)elapsed.count() / 1000 << '\n';
  size_t fn = 0;
  for (auto x : xs)
  {
    if (!bs.contains(x))
      ++fn;
  }

  std::cout << "sampled false negative rate: " << (double)fn/n << "\n";
  std::cout << "measured fn rate: " << bs.false_negative_rate() << "\n";

  size_t fp = 0;
  auto ys = random_strings(n);
  for (auto y : ys)
  {
    if (bs.contains(y))
      ++fp;
  }

  std::cout << "estimated false positive rate: " << (double)fp/n << "\n";
  std::cout << "theoretical fp rate: " << bs.false_positive_rate() << "\n";
}

