#pragma once

#include <hash_set/hash_set.hpp>
#include <algorithm>
#include <limits>
#include <chrono>
#include <vector>
#include <iostream>

namespace bernoulli
{
  template <typename H>
  struct hash_set_lvl2_builder
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

    std::ostream & debug_out;
    bool debug;
    size_t M;
    H h;
    size_t lower_index;
    size_t upper_index;
    double fpr;
    std::chrono::milliseconds duration;

    hash_set_lvl2_builder() :
      debug_out(std::cout),
      debug(false),
      M(0),
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

      if (M == 0)
        M = (size_t)std::ceil(std::sqrt(m));

      std::vector<size_t> sigma(M);

      // find l_star that most evenly divides the elements into the M sigma bins
      size_t l_star;
      double W = 0;
      auto const start_time = std::chrono::system_clock::now();
      
      for (auto l = lower_index; l != upper_index; ++l)
      {
        auto const elapsed =
          std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now() - start_time);
        if (elapsed > duration)
          break;
        
        std::vector<size_t> counts(M,0);
        for (auto x = begin; x != end; ++x)
          ++counts[h.mix(h(l),*x) % M];

        double S = 0;
        for (auto n : counts)
        {
          if (n == 0)
            continue;
          S -= std::log(n/m)*(double)n/m;
        }

        if (S > W)
        {
          l_star = l;
          W = S;
          if (debug)
            debug_out << "l* = " << l_star << "\n";

          if (abs(S - std::log(m)) < 1)
            break;
        }

        std::vector<std::vector<size_t>> hashes(M);
        for (auto x : xs)
        {
          hashes[h.mix(h(l_star),*x) % M];
        }
      }

      return hash_set_lvl2<H>(N,h,l_star,sigma,(double)(m-succ_star)/m);
    }

    template <typename X>
    auto operator()(X xs)
    {
      return operator()(std::begin(xs), std::end(xs));
    }
  };
}
