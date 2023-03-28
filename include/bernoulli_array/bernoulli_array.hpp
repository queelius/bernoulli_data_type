#pragma once
#include <vector>
#include <array>
#include <optional>
#include <random>
#include <set>
using std::vector;
using std::optional;
using std::array;




/**
 * @brief a bernoulli model of arrays.
 * 
 * Since data may be represented by functions and functions may be
 * represented by data, we use the hash_map, a data structure that models
 * the concept of the bernoulli map, to represent array data.
 * 
 * Note that since the hash map may be encoded by {h,l,d}, we can
 * create, say, an bernoulli array of bernoulli arrays by creating
 * a hash map for {h,l,d} and using that to instantiate the hash map.
 * We see that we can compose these data structures together, like in
 * most programming, to design new data structures.
 * 
 * @tparam H hash function type
 * @tparam D decoder function type
 */

template <typename H, typename D>
struct bernoulli_array
{
  hash_map<H,D> f;

  using value_type = typename D::value_type;

  auto operator[](size_t index)
  {
    return f(index);
  }
};

