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
}

