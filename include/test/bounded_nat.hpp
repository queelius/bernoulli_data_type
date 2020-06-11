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
    explicit bounded_nat(int x) : val_(x)
    {
        if (x < 0)
            throw invalid_argument("Natural number must be >= 0.");
    }

    explicit bounded_nat(unsigned int x = 0) : val_(x) {}

    explicit bounded_nat(size_t x = 0) : val_(x) {}

    bounded_nat(bounded_nat const & copy) : val_(copy.val_) {}

    operator unsigned int() const { return val_; };

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
        if (val_ > (long double)max() / rhs.val_)
            throw invalid_argument("Pair of natural numbers is not in the domain of definition of * : bounded_nat, bounded_nat -> bounded_nat. (overflow)");

        return bounded_nat(val_ * rhs.val_);
    }

    auto operator/(bounded_nat const & rhs) const
    {
        if (val_ % rhs.val_ != 0)
            throw invalid_argument("Pair of natural numbers is not in the domain of definition of / : bounded_nat, bounded_nat -> bounded_nat. (not closed)");

        return bounded_nat(val_ / rhs.val_);
    }

    static bounded_nat const & max()
    {
        static bounded_nat max_ = bounded_nat(numeric_limits<unsigned int>::max());
        return max_;
    }

private:
    unsigned int val_;
};



