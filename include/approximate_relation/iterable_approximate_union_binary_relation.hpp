#include "../forward_const_join_iterator.hpp"

namespace alex::approximate_binary_relation {

template<typename L, typename R>
class iterable_approximate_union_binary_relation
{
  public:
    // should be a pair type of some kind
    // right_type should be convertible to left_type,
    // so we just assume the left_type is the type of the union.

    using left_type = typename L::left_type;
    using right_type = typename L::right_type;
    using value_type = typename L::value_type;

    auto begin() const
    {
        return alex::iterators::make_forward_const_join_iterator(
          _l.begin(), _l.end(), _r.begin());
    };

    auto end() const
    {
        return alex::iterators::make_forward_const_join_iterator(
          _r.end(), _l.end(), _r.begin());
    };

    bool operator()(value_type const& p) const
    {
        return _l(p.first, p.second) || _r(p.first, p.second);
    };

    bool operator()(left_type a, right_type b) const
    {
        return _l(a, b) || _r(a, b);
    };

    // a relation is basically a function of the form
    //   R : left_type -> 2^right_type
    std::set<right_type> operator()(left_type x) const
    {
        std::set<right_type> ys;
        for (auto [a, b] : _l) {
            if (a == x) ys.insert(b);
        }
        for (auto [a, b] : _r) {
            if (a == x) ys.insert(b);
        }
        return ys;
    };

  private:
    L _l;
    R _r;
};

}