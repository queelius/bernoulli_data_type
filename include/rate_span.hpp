#pragma once

#include <algorithm>
using std::min;
using std::max;

/**
 * Models an uncertain rate. It is an interval type.
 */
class rate_span
{
public:
    using value_type = double;

    rate_span() : lb(1),ub(-1) {}

    rate_span(rate_span const & copy) = default;

    rate_span(value_type r) :
        lb(max(r,value_type(0))),
        ub(min(r,value_type(1))) {}

    rate_span(value_type r1, value_type r2) :
        lb(max(value_type(0),min(r1,r2))),
        ub(min(value_type(1),max(r1,r2))) {}

    rate_span(value_type r1, value_type r2, value_type r3) :
        lb(max(value_type(0),min(r1,r2))),
        ub(min(value_type(1),max(r1,r2))) {}

    auto min() const { return lb; }
    auto max() const { return ub; }

    auto contains(value_type r) const { return r >= min() && r <= max(); }
    auto empty() const { return max() < min(); }

private:
    value_type const lb, ub;
};

auto empty(rate_span const & x)
{
    return x.empty();
}

auto contains(rate_span const & x, value_type r)
{
    return x.contains(r);
}

auto degenerate(rate_span const & x)
{
    return min(x) == max(x);
}

auto min(rate_span const & x)
{
    return x.min();
}

auto max(rate_span const & x)
{
    return x.max();
}

bool subset(
    rate_span const & l,
    rate_span const & r)
{
    return min(l) >= min(r) &&
           max(l) <= max(r);
}

auto operator+(
    rate_span const & l,
    rate_span const & r)
{
    return rate_span(min(l)+min(r),max(l)+max(r));
}

auto operator-(
    rate_span const & l,
    rate_span const & r)
{
    return rate_span(min(l)-max(r),max(l)-min(r));
}

auto operator*(
    rate_span const & l,
    rate_span const & r)
{
    auto const x1 = min(l)*min(r);
    auto const x2 = min(l)*max(r);
    auto const x3 = max(l)*min(r);
    auto const x4 = max(l)*max(r);

    return rate_span(
        min(x1,min(x2,min(x3,x4))),
        max(x1,max(x2,max(x3,x4))));
}

auto operator&(
    rate_span const & l,
    rate_span const & r)
{
    return rate_span(
        max(min(l),min(r)),
        min(max(l),max(r)));
}

auto operator|(
    rate_span const & l,
    rate_span const & r)
{
    return rate_span(
        min(min(l),min(r)),
        max(max(l),max(r)));
}

