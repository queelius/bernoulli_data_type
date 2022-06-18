#pragma once

#include <bernoulli/hash_set.hpp>
#include <algorithm>
#include <limits>
#include <chrono>

#include <iostream>

namespace bernoulli
{
  template <typename H>
  struct hash_set_builder
  {
    static auto max_index()
    {
      return std::numeric_limits<size_t>::max();
    }

    static auto min_index()
    {
      return (size_t)0;
    }

    static auto min_false_positive_rate()
    {
      return 1.0 / std::numeric_limits<size_t>::max();
    }

    static auto max_false_positive_rate()
    {
      return 1.0;
    }

    static auto max_timeout()
    {
      return std::chrono::milliseconds::max().count();
    }

    static auto default_false_positive_rate()
    {
      return 1.0/1024; // approx 1e-3 = 0.001
    }

    std::ostream debug_out;
    bool debug;
    H h;
    size_t lower_index;
    size_t upper_index;
    double fpr;
    std::chrono::milliseconds duration;

    hash_set_builder() :
      debug_out(std::cout),
      debug(false),
      lower_index(min_index()),
      upper_index(max_index()),
      duration(max_timeout()),
      fpr(default_false_positive_rate()) {}
      
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
     * @brief Set the target false positive rate, a value in the interval (0,1].
     * @param r the target false positive rate of the bernojlli set.
     *          if such a bernoulli set cannot be constructed, then the
     *          false positive rate has r as a lower-bound.
     */
    auto & false_positive_rate(double r)
    {
      fpr = std::min(max_false_positive_rate(),
                     std::max(r, min_false_positive_rate()));
      return *this;
    }

    /**
     * @brief Set the hash function object.
     * @param h hash function
     */
    auto & hash_fn(H h)
    {
      this->h = h;
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
     * @tparam I models a forward iterator
     * @param begin start of range
     * @param end end of range
     */
    template <typename I>
    auto operator()(I begin, I end)
    {
      auto N = (size_t)std::round(fpr * std::numeric_limits<size_t>::max());
      std::sort(begin,end);
      end = std::unique(begin,end);
      auto m = std::distance(begin,end);

      size_t l_star;
      size_t succ_star = 0;
      auto const start_time = std::chrono::system_clock::now();

      for (auto l = lower_index; l != upper_index; ++l)
      {
        auto const cur_time = std::chrono::system_clock::now();
        auto const elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
          cur_time - start_time);
        if (succ_star == m || elapsed > duration)
          break;
          
        size_t succ = 0;
        for (auto x = begin; x != end; ++x)
          if (h.mix(l,h(*x)) <= N) ++succ;

        if (succ > succ_star)
        {
          if (debug)
          {
              debug_out << "best succ = " << succ << "\n";
              debug_out << "best l = " << l << "\n";
          }
          l_star = l;
          succ_star = succ;
        }
      }

      return hash_set<H>(N,h,l_star,(double)(m-succ_star)/m);
    }

    template <typename X>
    auto operator()(X xs)
    {
      return operator()(std::begin(xs), std::end(xs));
    }
  };
}
