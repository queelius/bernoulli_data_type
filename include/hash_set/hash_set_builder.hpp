#pragma once

#include <hash_set/hash_set.hpp>
#include <algorithm>
#include <limits>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <pthread.h>
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
      return 1.0 / max_index();
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
      return 1.0/1024.0; // approximately .0011
    }

    static auto default_threads()
    {
      return std::thread::hardware_concurrency();
    }

    std::ostream & debug_out;
    bool debug;
    H h;
    size_t lower_index;
    size_t upper_index;
    double fpr;
    size_t num_threads;
    std::chrono::milliseconds duration;

    hash_set_builder() :
      debug_out(std::cout),
      debug(false),
      lower_index(min_index()),
      upper_index(max_index()),
      fpr(default_false_positive_rate()),
      num_threads(default_threads()),
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
     * @brief Sets the number of threads.
     * 
     * @param n number of threads, defaults to 0, in which case system
     *          automatically chooses a number.
     */
    auto & threads(size_t n = 0)
    {
      num_threads = (n == 0 ? std::thread::hardware_concurrency() : n);
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

      size_t l0;
      size_t succ0 = 0;
      auto const stime = std::chrono::system_clock::now();
      std::mutex lck;

      auto task = [&](size_t start, size_t stop)
      {
        for (auto s = start; s != stop; ++s)
        {
          auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now() - stime);
          if (succ0 == m || elapsed > duration)
            break;

          size_t l = h(s);
          size_t succ = 0;
          for (auto x = begin; x != end; ++x)
          {
              //if ((l ^ h(*x)) <= N) ++succ;
              if (h.mix(l,*x) <= N) ++succ;
          }            

          if (succ > succ0)
          {
            lck.lock();
            if (succ > succ0)
            {
                l0 = l;
                succ0 = succ;

                if (debug)
                {
                    debug_out << "succ0: " << succ0 << "\n";
                    debug_out << "l0: " << l0 << "\n";
                }
            }
            lck.unlock();
          }
        }
      };

      size_t block_size = (upper_index - lower_index) / num_threads;

      if (debug)
      {
        std::cout << "num_threads: " << num_threads << "\n";          
        std::cout << "lower_index: " << lower_index << "\n";
        std::cout << "upper_index: " << upper_index << "\n";
        std::cout << "block_size: " << block_size << "\n";
        std::cout << "N: " << N << "\n";
      }

      std::vector<std::thread> threads;
      for (int i = 0; i < num_threads; ++i)
        threads.push_back(std::thread(task,lower_index+i*block_size,
                                      lower_index+(i+1)*block_size));
      for (auto & t : threads)
        t.join();      

      return hash_set<H>(N,h,l0,(double)(m-succ0)/m);
    }

    template <typename X>
    auto operator()(X xs)
    {
      return operator()(std::begin(xs), std::end(xs));
    }
  };
}
