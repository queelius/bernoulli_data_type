#pragma once

namespace alex::set
{
template <typename S> // models an iterable set type
class frozen_iterable_universal_set
{
public:
    using value_type = typename S::value_type;
    using set_type = S;
    using cardinality_type = size_t;

    operator S() const { return _s; };

    frozen_iterable_universal_set(S s) : _s(std::move(s)) {};

    constexpr bool contains(value_type const &) const { return true; };
    constexpr cardinality_type cardinality() const { return _s.cardinality(); };

    auto cbegin() const { return _s.cbegin(); };
    auto cend() const { return _s.cend(); };

private:
    S _s;
};

template <
    typename S1,
    typename S2>
bool subset(frozen_iterable_universal_set<S1> const & a, S2 const & b)
{
    return a.cardinality() == b.cardinality();
};

template <typename S>
auto make_frozen_iterable_universal_set(S const & s)
{
    return frozen_iterable_universal_set<S>(s);
};

}