#pragma once

#include <hash_map/hash_map.hpp>
#include <algorithm>
#include <limits>
#include <chrono>
#include <iostream>

namespace bernoulli
{
  /**
   * @brief The bernoulli map builder
   *        for a function of type Hashable(H) -> D::value_type.
   *
   * The builder does not need encoders,
   * only the decoder. It essentially does
   * the encoding of values in the codomain
   * by finding a hashing index
   * the generates an appropriate encoding
   * where appropriate is defined as when
   * we apply the decoder to it, we get back
   * the expected value in the codomain.
   *
   * @tparm H hash function object type
   * @tpaam D prefix-free decoder function object type
   */
  template <typename H, typename D>
  struct hash_map_builder
  {
    static auto max_index()
    {
      return std::numeric_limits<size_t>::max();
    }

    static auto min_index()
    {
      return (size_t)0;
    }

    static auto max_timeout()
    {
      return std::chrono::milliseconds::max().count();
    }

    std::ostream & debug_out;
    bool debug;
    H h;
    D d;
    size_t lower_index;
    size_t upper_index;
    std::chrono::milliseconds duration;

    hash_map_builder() :
      debug_out(std::cout),
      debug(false),
      lower_index(min_index()),
      upper_index(max_index()),
      duration(max_timeout()) {}

    /**
     * @brief Set debug mode to true or false.
     * @param mode if mode is true, then show debugging information.
     */
    auto & debugging(bool mode = true)
    {
      debug = mode;
      return *this;
    }    
    
    /**
     * @brief Set debug output.
     * @param out the debugging output stream.
     */
    auto & debug_output(std::ostream & out)
    {
      debug_out = out;
      return *this;
    }

    /**
     * @brief Set the hash function object.
     * @param h hash function object
     */
    auto & hash_fn(H h)
    {
      this->h = h;
      return *this;
    }

    /**
     * @brief Set the decoder function object.
     * @param d decoder function object
     */
    auto & decoder_fn(H h)
    {
      this->d = d;
      return *this;
    }

    /**
     * @brief Sets the index range [lower,upper], the space over which to search
     *        for a perfect hash function.
     *
     * @param lower start of search space
     * @param upper end of the search space
     */
    auto & index(size_t lower = min_index(),
                 size_t upper = max_index())
    {
      lower_index = lower;
      upper_index = upper;
      return *this;
    }

    /**
     * @brief Set the time-out, the length of time before setttling for the
     *        best solution found.     * 
     * 
     * @param ms the time-out in milliseconds
     */
    auto & timeout(std::chrono::milliseconds ms)
    {
      duration = ms;
      return *this;
    }

    /**
     * @brief Constructs a random perfect hash function for the range of
     *        elements in [begin,end).
     *
     * Each element in [begin,end) models the concept of a pair, where
     * car(*begin) maps to a value in the domain of the function f
     * and cdr(*begin) maps to a value in the codomain of the function f
     * such that f(car(*begin)) = cdr(*begin).
     *
     * Precondition: [begin,end) does not include any pairs such that
     * its car is not unique.
     *
     * @tparam I models a forward iterator
     * @param begin start of range
     * @param end end of range
     */
    template <typename I>
    auto operator()(I begin, I end)
    {
      auto m = std::distance(begin,end);

      size_t l_star;
      size_t succ_star = 0;
      auto const start_time = std::chrono::system_clock::now();

      for (auto l = lower_index; l != upper_index; ++l)
      {
        auto const elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now() - start_time);
        if (succ_star == m || elapsed > duration)
          break;

        size_t succ = 0;
        for (auto x = begin; x != end; ++x)
        {
          if (d(h.mix(h(*x),l)) == *x)
            ++succ;
        }

        if (succ > succ_star)
        {
          if (debugging)
          {
            std::cout << "best succ = " << succ << "\n";
            std::cout << "best l = " << l << "\n";
          }
          l_star = l;
          succ_star = succ;
        }
      }

      return hash_map<H,D>(h,d,l_star,(double)(m-succ_star)/m);
    }

    template <typename X>
    auto operator()(X xs)
    {
      return operator()(std::begin(xs), std::end(xs));
    }
  };
}
