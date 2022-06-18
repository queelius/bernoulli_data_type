#include <memory>

namespace bernoulli
{
  // R models a bernoulli symmetric relation.
  template<typename R>
  struct symmetric_bernoulli_relation
  {
    using X = typename R::left_value_type;
    using Y = typename R::right_value_type;

    public:
      // value_type should be some sort of pair type
      // where each element is of the same type,
      // otherwise symmetry should not be possible.
      using value_type = std::pair<X,Y>;

      bool 

      // we enforce the invariant that (x, y) in R
      // iff (y, x) in R.
      bool contains(X const & x, Y const & y) const
      {
          return r.contains(x,y) || r.contains(y,x);
      }

      R r;
  };
}