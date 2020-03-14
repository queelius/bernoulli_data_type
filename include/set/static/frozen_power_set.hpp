#include <set>

namespace alex::set
{
  template <
    typename S,
    template <typename> typename IterableValueContainerType = std::set>
  class frozen_power_set
  {
  public:
    using set_type = S;
    using value_type = IterableValueContainerType<typename S::value_type>;

    frozen_power_set(S s) : _s(std::move(s)) {};

    /**
     * We do not actually care about the container for the value_type as
     * long as it has cbegin and cend (which return iterators) for it.
     * We provide a definition for value_type for other algorithms that
     * need meta-information about the value type of the frozen power set.
     */
    template <typename IterableContainer>
    bool contains(IterableContainer const & x) const
    {
      return std::all_of(cbegin(x), cend(x),
        [_s](auto const & y) { return _s.contains(y); });
    };

    private:
      S _s;
  };
}