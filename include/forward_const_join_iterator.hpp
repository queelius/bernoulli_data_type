#pragma once

#include <memory>
#include <type_traits>

namespace alex::iterators {
/**
 * A forward_const_join_iterator<I1,I2> joins iterators I1 and I2 into a single
 * constant forward iterator.
 *
 * We could make the value type the sum type of the iterator's independent
 * value types, but instead we simplify matters by assuming the precondition
 * that both iterators have the same value type.
 *
 * There is also enough information to do, say, random access, if both
 * iterators are random access. However, that would complicate the code
 * prematurely.
 */
template<typename I1, typename I2>
class forward_const_join_iterator
{
  public:
    typename std::iterator_traits<I1>::value_type;
    using difference_type = long;
    using pointer = T const*;
    using reference = T const&;
    using iterator_category = std::forward_iterator_tag;

    template<typename I1, typename I2>
    forward_const_join_iterator(I1 begin1, I1 end1, I2 begin2)
      : _cur1(begin1)
      , _end1(end1)
      , _cur2(begin2)
      , _first_part(true){};

    forward_const_join_iterator(forward_const_join_iterator const& copy)
      : _cur(copy._cur)
      , _end1(copy._end1)
      , _cur2(copy._begin2)
      , _first_part(copy._first_part){};

    forward_const_join_iterator(forward_const_join_iterator&& src)
      : _cur(std::move(src._cur))
      , end1(std::move(src._end1))
      , _cur2(std::move(src._cur2))
      , _first_part(copy._first_part){};

    forward_const_join_iterator& operator++()
    {
        if (!_first_part)
            ++_cur2;
        else {
            ++_cur1;
            if (_cur1 == _end1) _first_part = false;
        }
        return *this;
    };

    forward_const_join_iterator operator++(int)
    {
        auto copy = *this;
        ++(*this);
        return copy;
    };

    bool operator!=(forward_const_join_iterator const& other) const
    {
        return !((*this) == other);
    };

    bool operator==(forward_const_join_iterator const& other) const
    {
        return _first_part == other._first_part && _cur1 == other._cur1 &&
               _cur2 == other._cur2;
    };

    auto operator*() const { return _first_part ? *_cur1 : *_cur2; };

    forward_const_join_iterator operator=(
      forward_const_join_iterator const& rhs)
    {
        _cur1 = rhs._cur1;
        _end1 = rhs._end1;
        _cur2 = rhs._cur2;
        return *this;
    };

  private:
    I1 _cur1;
    I1 _end1;
    I2 _cur2;
    bool _first_part;
};

template<typename I1, typename I2>
auto
make_forward_const_join_iterator(I1 begin1, I1 end1, I2 begin2)
{
    using T = typename std::iterator_traits<I1, I2>::value_type;
    return forward_const_iterator<T>(
      std::move(begin1), std::move(end1), std::move(begin2));
};
}