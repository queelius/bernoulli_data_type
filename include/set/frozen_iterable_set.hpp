#pragma once

#include <memory>
#include "../../forward_const_iterator.hpp"

namespace alex::set
{
using std::cbegin;
using std::cend;
using std::move;

/**
 * Type-erasure for frozen (immutable) iterable sets with elements of type T.
 * 
 * For an object of type S to be type-erasable as frozen_iterable_set<T>, it
 * must overload the following set of functions:
 *   cardinality : S -> cardinality_type
 *   contains : (S,T) -> bool
 *   cbegin : S -> Iterator
 *   cend : S -> Iterator
 * where given an instance s of S, iterator cend(s) is reachable from iterator
 * cbegin(s).
 * 
 * The frozen_iterable_set<T> supports the following functions:
 *   cardinality : frozen_iterable_set<T> -> cardinality_type
 *   contains : (frozen_iterable_set<T>, T) -> bool
 *   cbegin : frozen_iterable_set<T> -> forward_const_iterator<T>
 *   cend : frozen_iterable_set<T> -> forward_const_iterator<T>
 * where forward_const_iterator<T> is a constant forward iterator with
 * value type T.   
 */
template <typename T>
class frozen_iterable_set
{
public:
  using value_type = T;
  using cardinality_type = size_t;

  template <typename Set>
  frozen_iterable_set(Set s) : _s(std::make_shared<concept_model<Set>>(
                                   move(s))){};

  cardinality_type cardinality() const
  {
    return _s->cardinality();
  };

  bool contains(T const &x) const
  {
    return _s->contains(x);
  };

  forward_const_iterator<T> cbegin() const
  {
    return _s->cbegin();
  };

  forward_const_iterator<T> cend() const
  {
    return _s->cend();
  };

private:
  struct concept
  {
    virtual bool contains(T const &) const = 0;
    virtual cardinality_type cardinality() const = 0;
    virtual forward_iterator_const<T> cbegin() const = 0;
    virtual forward_iterator_const<T> cend() const = 0;
  };

  template <typename Set>
  struct concept_model final : concept
  {
    concept_model(Set s) : s(std::move(s)){};

    cardinality_type cardinality() const
    {
      return static_cast<cardinality_type>(cardinality(s));
    };

    bool contains(T const &x) const
    {
      return contains(s, x);
    };

    forward_iterator_const<T> cbegin() const
    {
      return alex::iterators::make_forward_const_iterator(cbegin(s));
    };

    forward_iterator_const<T> cend() const
    {
      return alex::iterators::make_forward_const_iterator(cend(s));
    };

    Set s;
  };

  std::shared_ptr<concept const> _s;
};

template <typename T>
auto cardinality(frozen_iterable_set<T> const &s)
{
  return _s->cardinality();
};

template <typename T>
bool contains(frozen_iterable_set<T> const &s, T const &x)
{
  return _s->contains(x);
};

template <typename T>
alex::iterators::forward_const_iterator<T>
cbegin(frozen_iterable_set<T> const &s)
{
  return _s->cbegin();
};

template <typename T>
alex::iterators::forward_const_iterator<T>
cend(frozen_iterable_set<T> const &s)
{
  return _s->cend();
};

template <typename T>
bool subset(frozen_iterable_set<T> const &a, frozen_iterable_set<T> const &b)
{
  auto it = a.iterator();
  while (it.has_next())
  {
    if (!b.contains(it.value()))
      return false;
  }
  return true;
};

template <typename T>
bool equal(frozen_iterable_set<T> const &a, frozen_iterable_set<T> const &b)
{
  if (a.cardinality() != b.cardinality())
    return false;
  return subset(a, b);
};

template <typename T>
bool operator==(
    frozen_iterable_set<T> const &a,
    frozen_iterable_set<T> const &b)
{
  return equal(a, b);
};
} // namespace alex::set