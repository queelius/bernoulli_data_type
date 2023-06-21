#pragma once

#include <algorithm>

/**
 * Models an uncertain rate. It is an interval type.
 */
struct rate_span
{
    rate_span() : min(0.0f), max(1.0f) {}
    rate_span(float r) : min(std::max(r,0.0f)), max(std::min(r,1.0f)) {}
    rate_span(float r1, float r2)
        : min(std::max(0.0f,std::min(r1,r2))), 
          max(std::min(1.0f,std::max(r1,r2))) {}

    const float min, max;
};

auto min(rate_span x) {  return x.min; }
auto max(rate_span x) {  return x.max; }

auto operator+(rate_span l, rate_span r)
{
    return rate_span(l.min+r.min, l.max+r.max);
}

auto operator-(rate_span l, rate_span r)
{
    return rate_span(l.min-r.max, l.max-r.min);
}

auto operator*(rate_span l, rate_span r)
{
    auto x1 = l.min * r.min;
    auto x2 = l.min * r.max;
    auto x3 = l.max * r.min;
    auto x4 = l.max * r.max;

    return rate_span(std::min({x1,x2,x3,x4}),
                     std::max({x1,x2,x3,x4}));
}

auto operator&(rate_span l, rate_span r)
{
    return rate_span(std::max(l.min,r.min), std::min(l.max,r.max));
}

auto operator|(rate_span l, rate_span r)
{
    return rate_span(std::min(l.min,r.min), std::max(l.max,r.max));
}
