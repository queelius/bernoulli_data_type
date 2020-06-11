#pragma once

#include <optional>
#include <numeric>
#include <stdexcept>
#include <limits>

using std::invalid_argument;
using std::numeric_limits;

// bounded natural number
// template <typename T> // T models a semiring with opeprator set {+,-,/,*}
//                       // and relational predicates <,<=,>,>=,==,!=
class bounded_nat
{
public:
    bounded_nat(int x) : val_(x)
    {
        if (x < 0)
            throw invalid_argument("Natural number must be >= 0.");
    }

    bounded_nat(unsigned int x = 0) : val_(x) {}

    bounded_nat(double x) : bounded_nat((long double)x) {}

    bounded_nat(bounded_nat const & copy) : val_(copy.val_) {};

    bounded_nat(long double x)
    {
        if (x < 0)
            throw invalid_argument("Natural number must be >= 0.");

        if ((unsigned int)x != x)
            throw invalid_argument("Natural number must be a value in the set {0,1,2,...}.");
    }

    unsigned int get() const { return val_; };

    auto operator<(bounded_nat const & rhs) const { return val_ < rhs.val_; }
    auto operator>(bounded_nat const & rhs) const { return val_ > rhs.val_; }
    auto operator<=(bounded_nat const & rhs) const { return val_ <= rhs.val_; }
    auto operator>=(bounded_nat const & rhs) const { return val_ >= rhs.val_; }
    auto operator==(bounded_nat const & rhs) const { return val_ == rhs.val_; }
    auto operator!=(bounded_nat const & rhs) const { return val_ != rhs.val_; }

    auto operator+(bounded_nat const & rhs) const
    {
        if (val_ > max().val_ - rhs.val_)
            throw invalid_argument("Pair of natural numbers is not in the domain of definition of + : bounded_nat, bounded_nat -> bounded_nat. (overflow)");

        return bounded_nat(val_ + rhs.val_);
    }

    auto operator*(bounded_nat const & rhs) const
    {
        if (val_ > (long double)max().val_ / rhs.val_)
            throw invalid_argument("Pair of natural numbers is not in the domain of definition of * : bounded_nat, bounded_nat -> bounded_nat. (overflow)");

        return bounded_nat(val_ * rhs.val_);
    }

    auto operator/(bounded_nat const & rhs) const
    {
        if (val_ % rhs.val_ != 0)
            throw invalid_argument("Pair of natural numbers is not in the domain of definition of / : bounded_nat, bounded_nat -> bounded_nat. (not closed)");

        return bounded_nat(val_ / rhs.val_);
    }

    pair<bounded_nat,bounded_nat> div(bounded_nat const & rhs) const
    {
        auto remainder = _val % rhs._val;
        auto quotient = _val / rhs._val;

        return make_pair(quotient, remainder);
    }

    static bounded_nat const & max()
    {
        static bounded_nat max_ = bounded_nat(numeric_limits<unsigned int>::max());
        return max_;
    }

private:
    unsigned int val_;
};



