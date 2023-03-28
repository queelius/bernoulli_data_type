#pragma once

#include <limits>
#include <cmath>

namespace coders
{
  struct bool_decoder
  {
      using value_type = bool;

      auto implicit_probability(bool x)
      {
          static auto const p =
            (double)k / std::numeric_limits<size_t>::max();
          return x ? p : 1-p;
      }

      bool_decoder(bool_decoder const &) = default;
      bool_decoder(size_t k) : k(k) {}

      auto operator()(size_t code) const { return code <= k; }
      size_t const k;
  };

  struct bool_encoder
  {
      using value_type = bool;

      auto implicit_probability(bool x)
      {
          static auto const p =
            (double)k / std::numeric_limits<size_t>::max();
          return x ? p : 1-p;
      }

      bool_encoder(bool_encoder const &) = default;
      bool_encoder(size_t k) : k(k) {}

      auto operator()(bool x) const { return x ? 1 : (k+1); }
      size_t const k;
  };
}