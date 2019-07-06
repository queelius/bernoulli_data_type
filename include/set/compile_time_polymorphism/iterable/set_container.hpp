#pragma once

#include <set>

namespace alex::set::iterable
{

template <
    typename T,
    template <typename> typename C = std::set>
class Set
{
public:
    using value_type = T;
    using cardinality_type = decltype(std::declval<C<T>>().size());
    using container_set_type = C<T>;
    using const_iterator = decltype(std::declval<C<T>>().cbegin());

    Set(std::initializer_list<T> s) : _s(s.begin(), s.end()) {};
    Set(C<T> const & s) : _s(s) {};
    Set() = default;
    Set(Set<T> const & s) : _s(s._s) {};
    Set(Set<T> && s) : _s(std::move(s._s)) {};

    void insert(T x) { _s.insert(x); };

    cardinality_type cardinality() const { return _s.size(); };

    // provide overloads so that if C<T> has any variation
    // it will work.
    bool contains(T const& x) const { return _s.count(x) != 0; };

    auto cbegin() const { return _s.cbegin(); };
    auto cend() const { return _s.cend(); };
    auto begin() const { return _s.cbegin(); };
    auto end() const { return _s.cend(); };

    bool operator<(Set<T> const & other) const
    {
        return _s < other._s;
    };

    bool operator==(Set<T> const & other) const
    {
        return _s == other._s;
    };


private:
    std::set<T> _s;
};

template <typename T, typename S>
bool subset_equal(Set<T> const & a, S const & b)
{
    for (auto const & x : a)
        if (!b.contains(x))
            return false;
    return true;
};

template <typename T, typename S>
bool equal(Set<T> const & a, S const & b)
{
    if (a.cardinality() != b.cardinality())
        return false;
    return subset_equal(a, b);
};


}