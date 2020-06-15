#pragma once

#include <stdexcept>
using std::invalid_argument;

template <unsigned int K>
class nat
{
public:
    explicit nat(int x) : val_(x)
    {
        if (x < 0)
            throw invalid_argument("Underflow");
        if (x > K)
            throw invalid_argument("Overflow");
    }

    explicit nat(size_t x) : val_(x)
    {
        if (x > K)
            throw invalid_argument("Overflow");
    }

    nat(unsigned int x) : val_(x)
    {
        if (x > K)
            throw invalid_argument("Overflow");
    }

    nat(nat const & copy) : val_(copy.val_) {}

    nat() : val_(0) {}

    operator unsigned int() const { return val_; };
    operator size_t() const { return val_; };
    operator int() const { return val_; };
    operator double() const { return val_; };

    nat operator--(int) { return (*this)-nat((unsigned int)1); }
    nat operator++(int) { return (*this)+nat((unsigned int)1); }

    nat & operator--()
    {
        if (val_ == 0) throw invalid_argument("Underflow");
        --val_;
        return *this;
    }

    nat operator++()
    {
        if (val_ == K) throw invalid_argument("Overflow");
        ++val_;
        return *this;
    }

private:
    unsigned int val_;
};

template <unsigned int K>
auto operator<(nat<K> const & lhs, nat<K> const & rhs) { return lhs.val_ < rhs.val_; }

template <unsigned int K>
auto operator>(nat<K> const & lhs, nat<K> const & rhs) { return lhs.val_ > rhs.val_; }

template <unsigned int K>
auto operator<=(nat<K> const & lhs, nat<K> const & rhs) { return lhs.val_ <= rhs.val_; }

template <unsigned int K>
auto operator>=(nat<K> const & lhs, nat<K> const & rhs) { return lhs.val_ >= rhs.val_; }

template <unsigned int K>
auto operator==(nat<K> const & lhs, nat<K> const & rhs) { return lhs.val_ == rhs.val_; }

template <unsigned int K>
auto operator!=(nat<K> const & lhs, nat<K> const & rhs) { return lhs.val_ != rhs.val_; }

template <unsigned int K>
auto operator+(nat<K> const & lhs, nat<K> const & rhs)
{
    if (lhs.val_ > K - rhs.val_)
        throw invalid_argument("Overflow");

    return nat<K>(lhs.val_ + rhs.val_);
}

template <unsigned int K>
auto operator-(nat<K> const & lhs, nat<K> const & rhs)
{
    if (lhs.val_ < rhs.val_)
        throw invalid_argument("Underflow");

    return nat<K>(lhs.val_ - rhs.val_);
}

template <unsigned int K>
auto operator*(nat<K> const & lhs, nat<K> const & rhs)
{
    if (lhs.val_ > (long double)K / rhs.val_)
        throw invalid_argument("Overflow");

    return nat<K>(lhs.val_ * rhs.val_);
}

template <unsigned int K>
auto operator/(nat<K> const & lhs, nat<K> const & rhs)
{
    if (lhs.val_ % rhs.val_ != (unsigned int)0)
        throw invalid_argument("Outside the domain of definition.");

    return nat<K>(lhs.val_ / rhs.val_);
}

template <unsigned int K>
auto max(nat<K> const & lhs, nat<K> const & rhs)
{
    return (unsigned int)lhs < (unsigned int)rhs ? lhs : rhs;
}

template <unsigned int K>
auto min(nat<K> const &)
{
    return (unsigned int)lhs < (unsigned int)rhs ? rhs : lhs;
}
