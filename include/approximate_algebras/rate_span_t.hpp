#pragma once

#include <algorithm>
#include <limits>
using std::numeric_limits;
using std::min;
using std::max;

class rate_span_t
{
public:
    using value_type = double;

    rate_span_t() : lb(value_type{1}), ub(value_type{0}) {}

    rate_span_t(value_type r) :
        lb(max(r,value_type{0})),
        ub(min(r,value_type{1})) {}

    rate_span_t(value_type r1, value_type r2) :
        lb(max(value_type{0},min(r1,r2))),
        ub(min(value_type{1},max(r1,r2))) {}

    auto lower() const { return lb; }
    auto upper() const { return ub; }
    auto contains(value_type r) const { return r >= lower() && r <= upper(); }
    auto empty() const { return ub < lb; }

private:
    value_type const lb, ub;
};

auto min(rate_span_t const & x)
{
    return x.lower();
}

auto max(rate_span_t const & x)
{
    return x.upper();
}

auto operator+(
    rate_span_t const & l,
    rate_span_t const & r)
{
    return rate_span_t(min(l)+min(r),max(l)+max(r));
}

auto operator-(
    rate_span_t const & l,
    rate_span_t const & r)
{
    return rate_span_t(min(l)-max(r),max(l)-min(r));
}

auto operator*(
    rate_span_t const & l,
    rate_span_t const & r)
{
    auto const x1 = min(l)*min(r);
    auto const x2 = min(l)*max(r);
    auto const x3 = max(l)*min(r);
    auto const x4 = max(l)*max(r);

    return rate_span_t(
        min(x1,min(x2,min(x3,x4))),
        max(x1,max(x2,max(x3,x4))));
}

auto operator/(
    rate_span_t const & l,
    rate_span_t const & r)
{
    using T = rate_span_t::value_type;
    T y1 = min(r), y2 = max(r);
    return l * rate_span_t(
        y2 == T(0) ? -numeric_limits<T>::infinity() : T(1) / y2,
        y1 == T(0) ? numeric_limits<T>::infinity() : T(1) / y2);

}

auto operator&(
    rate_span_t const & l,
    rate_span_t const & r)
{
    return rate_span_t(
        max(min(l),min(r)),
        min(max(l),max(r)));
}

auto operator|(
    rate_span_t const & l,
    rate_span_t const & r)
{
    return rate_span_t(
        min(min(l),min(r)),
        max(max(l),max(r)));
}

bool subset(
    rate_span_t const & l,
    rate_span_t const & r)
{
    return min(l) >= min(r) &&
           max(l) <= max(r);
}

