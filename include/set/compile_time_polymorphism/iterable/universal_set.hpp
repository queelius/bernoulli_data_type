#pragma once

namespace alex::set::iterable
{
template <
    typename S,                 // models an iterable set type
    typename I = std::size_t    // models an integer type
>
class UniversalSet
{
public:
    using value_type = typename S::value_type;
    using set_type = S;
    using cardinality_type = I;

    operator S() const { return _s; };

    UniversalSet(S s) : _s(std::move(s)) {};
    UniversalSet(S&& s) : _s(std::move(s)) {};
    constexpr bool contains(value_type const &) const { return true; };
    constexpr I cardinality() const { return _s.cardinality(); };

    auto begin() const { return _s.begin(); };
    auto cbegin() const { return _s.cbegin(); };
    auto end() const { return _s.end(); };
    auto cend() const { return _s.cend(); };

private:
    S _s;
};

template <
    typename S1,
    typename S2,
    typename I>
bool subset_equal(UniversalSet<S1,I> const & a, S2 const & b)
{
    return a.cardinality() == b.cardinality();
};

template <typename S>
auto make_universal_set(S const & s)
{
    return UniversalSet<S>(s);
};

}